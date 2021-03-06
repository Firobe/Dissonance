#include <condition_variable>
#include <iomanip>
#include <ctime>

#include "DiscordAPI.hpp"

using namespace std;
mutex mtx;
condition_variable cv;
bool stop = false;

const http_response& statusCheck(const http_response& r, status_code expected, string name) {
	if (r.status_code() != expected) {
		string re = "No more information.";
		try {
			json::value j = r.extract_json().get();
			re = j.at("message").as_string();
		}catch(...){}
		throw runtime_error("ERROR : " + name + " received status code " +
				to_string(r.status_code()) + " instead of " +
				to_string(expected) + "\n" + re);
	}
	return r;
}

DiscordAPI::DiscordAPI() :
	_http_client("https://discordapp.com/api/"),
		_bot(nullptr) {
			_startTime = chrono::high_resolution_clock::now();
		}

void DiscordAPI::wait() {
	unique_lock<mutex> lck(mtx);
	while(!stop) cv.wait(lck);
	cerr << "API interrupted !" << endl;
	if(_closed) _web_client.close(websocket_close_status::going_away, "Manual close").wait();
}

void DiscordAPI::connect(DiscordBot* bot, string token) {
	stop = false;
	_closed = false;
	_dead = false;
	_initPhase = 0;
	_web_client = websocket_callback_client();
	_resuming = (_bot != nullptr);
	_bot = bot;
	_token = token;
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	cout << "DISSONANCE\n============================================" << endl;
	cout << "Current time : " << std::put_time(&tm, "%d/%m/%Y %H:%M:%S") << endl;
	//GET endpoint
	http_response endResp = statusCheck(
			httpRequest(methods::GET, "/gateway"),
			status_codes::OK, "Get Gateway");
	_wsEndpoint = endResp.extract_json().get()["url"].as_string();
	cout << "Getting gateway endpoint..." << endl;
	cout << "Gateway endpoint : " << _wsEndpoint << endl;
	//Connection
	uri_builder builder(_wsEndpoint);
	builder.append_query("v", 6);
	builder.append_query("encoding", "json");
	_web_client.set_message_handler(bind(&DiscordAPI::receiveAndDispatch, this, placeholders::_1));

	//Close manager
	_web_client.set_close_handler([this](websocket_close_status, string reason, error_code) {
			cerr << "Connection closed by Discord : " << reason << endl;
			stop = true;
			_closed = true;
			cv.notify_all();
			});

	cout << "Connecting to Gateway... " << endl;
	_web_client.connect(builder.to_string()).wait();
	cout << "Connection OK" << endl;
}

void DiscordAPI::helloHandler(json::value& hello) {
	//Hello
	if (hello["op"].as_integer() != 10)
		throw runtime_error("Bad Hello");

	_heartbeat = hello["d"]["heartbeat_interval"].as_integer();
	cout << "Hello ! Hearbeat = " << _heartbeat << " ms" << endl;
	if(!_resuming){
		_initPhase = 1;
		//Identify
		cout << "Identifying..." << endl;
		sendJson(gatewayPayload(identifyValue(), 2));
		cout << "Sent, awaiting answer..." << endl;
	}
	else {
		_initPhase = 2;
		cout << "Resuming connection..." << endl;
		json::value pay;
		pay["token"] = json::value(_token);
		pay["session_id"] = json::value(_sessionID);
		pay["seq"] = json::value(_seq);
		sendJson(gatewayPayload(pay, 6));
		finalHandler();
	}
}

void DiscordAPI::readyHandler(json::value& ready){
	if (ready["op"].as_integer() != 0 or ready["t"].as_string() != "READY")
		throw runtime_error("Bad Ready");
	_initPhase = 2;
	_seq = ready["s"].as_integer();
	auto event = ready["d"];
	_user = event["user"];
	_sessionID = event["session_id"].as_string();
	finalHandler();
}

void DiscordAPI::finalHandler() {
	cout << "Success !\n============================================" << endl;
	_keepAliveTask = thread(&DiscordAPI::keepAlive, this);
	cout << "Now listening..." << endl;
	_keepAliveTask.detach();
}

void DiscordAPI::keepAlive() {
	unique_lock<mutex> lck(mtx);
	try {
		while (!stop) {
			if (_dead)
				throw runtime_error("DEAD CONNECTION");

			json::value d(_seq);
			sendJson(gatewayPayload(d, 1));
			_dead = true;
			cv.wait_for(lck, chrono::milliseconds(_heartbeat));
		}
	}
	catch (exception& e) {
		cout << "KeepAlive : " << e.what() << endl;
		stop = true;
		cv.notify_all();
	}
}

void DiscordAPI::receiveAndDispatch(const websocket_incoming_message& msg) {
	try {
		json::value received = json::value::parse(msg.extract_string().get());
		if(_initPhase == 0)
			helloHandler(received);
		else if(_initPhase == 1)
			readyHandler(received);
		else {
			switch (received["op"].as_integer()) {
				case 0:
					_seq = received["s"].as_integer();
					eventDispatcher(received["t"].as_string(), received["d"]);
					break;

				case 3:
					cout << "Status update" << endl;
					break;

				case 11:
					_dead = false;
					break;

				case 9:
					throw runtime_error("Invalid session");

				default:
					cout << "Received unprocessed message " << received.serialize() << endl;
					cout << "OP was " << received["op"].as_integer() << endl;
			}
		}
	} catch(exception& e) {
		cout << "receiveAndDispatch : " << e.what() << endl;
		stop = true;
		cv.notify_all();
	}
}

void DiscordAPI::eventDispatcher(string name, json::value event) {
	try{
		if (name == "MESSAGE_CREATE")
			_bot->onMessageCreate(event);
		else if(name == "MESSAGE_DELETE")
			_bot->onMessageDelete(event);
		else if(name == "GUILD_CREATE")
			_bot->onGuildCreate(event);
		else if(name == "CHANNEL_CREATE")
			_bot->onChannelCreate(event);
		else if(name == "CHANNEL_UPDATE")
			_bot->onChannelUpdate(event);
		else if(name == "CHANNEL_DELETE")
			_bot->onChannelDelete(event);
		else if(name == "GUILD_MEMBER_ADD")
			_bot->onGuildMemberAdd(event["guild_id"].as_string(), event);
		else if(name == "GUILD_MEMBER_REMOVE")
			_bot->onGuildMemberRemove(event["guild_id"].as_string(),
					event["user"]);
		else if(name == "GUILD_MEMBER_UPDATE")
			_bot->onGuildMemberUpdate(event["guild_id"].as_string(),
					event);
		else if(name == "GUILD_ROLE_CREATE")
			_bot->onGuildRoleCreate(event["guild_id"].as_string(),
					event["role"]);
		else if(name == "GUILD_ROLE_UPDATE")
			_bot->onGuildRoleUpdate(event["guild_id"].as_string(),
					event["role"]);
		else if(name == "GUILD_ROLE_DELETE")
			_bot->onGuildRoleDelete(event["guild_id"].as_string(),
					event["role_id"].as_string());
		else if(name == "GUILD_BAN_ADD")
			_bot->onGuildBanAdd(event["guild_id"].as_string(), event["user"]);
		else if(name == "GUILD_BAN_REMOVE")
			_bot->onGuildBanRemove(event["guild_id"].as_string(), event["user"]);
	}
	catch(exception& e) {
		cerr << "Exception while processing event (" << name << ") : " << e.what() << endl;
		cerr << "Event was : " << event << endl;
		//throw e;
	}
}

json::value DiscordAPI::identifyValue() {
	json::value properties;
	properties["os"] = json::value("linux");
	properties["device"] = json::value("Dissonance");
	json::value d;
	d["token"] = json::value(_token);
	d["properties"] = properties;
	d["compress"] = json::value(false);
	d["large_threshold"] = json::value(250);
	return d;
}

json::value DiscordAPI::gatewayPayload(json::value d, int opcode) {
	json::value m;
	m["op"] = json::value(opcode);
	m["d"] = d;
	return m;
}

User DiscordAPI::getUser() {
	return _user;
}

chrono::minutes DiscordAPI::upTime() {
	return chrono::duration_cast<chrono::minutes>(chrono::high_resolution_clock::now() - _startTime);
}

void DiscordAPI::sendJson(json::value v) {
	string s(v.serialize());
	websocket_outgoing_message msg;
	msg.set_utf8_message(s);
	_web_client.send(msg).wait();
}

http_response DiscordAPI::httpRequest(const method& method, string endpoint, json::value body) {
	http_request request(method);
	request.set_request_uri(U(endpoint));
	request.headers().add("Authorization", "Bot " + _token);
	request.headers().add("User-Agent", "DiscordBot (Sakamoto, 6.9)");
	if(method != methods::GET) {
		request.headers().add("Content-Length", body.serialize().size());
		request.set_body(body);
	}
	http_response response = _http_client.request(request).get();
	return response;
}

/******************************************************
 *     API BEGINS HERE
 ******************************************************/


void DiscordAPI::sendMessage(Message me, string channelId) {
	json::value m = me.toJson();
	string endpoint = "/channels/" + channelId + "/messages";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::POST, endpoint, m);
	statusCheck(r, status_codes::OK, "sendMessage");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::deleteMessage(string channelId, string messageId) {
	string endpoint = "/channels/" + channelId + "/messages";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::DEL, endpoint + "/" + messageId);
	statusCheck(r, status_codes::NoContent, "deleteMessage");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::bulkDeleteMessages(string channelId, vector<string>& messageIds) {
	json::value arr, payload;
	for(unsigned i = 0 ; i < messageIds.size() ; ++i)
		arr[i] = json::value(messageIds[i]);
	payload["messages"] = arr;
	string endpoint = "/channels/" + channelId + "/messages/bulk-delete";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::POST, endpoint, payload);
	statusCheck(r, status_codes::NoContent, "bulkDeleteMessage");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::addGuildMemberRole(string guildId, string userId, string roleId) {
	string endpoint = "/guilds/" + guildId + "/members//roles";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::PUT, "/guilds/" + guildId + "/members/" + userId
			+ "/roles/" + roleId);
	statusCheck(r, status_codes::NoContent, "addGuildMemberRole");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::removeGuildMemberRole(string guildId, string userId, string roleId) {
	string endpoint = "/guilds/" + guildId + "/members//roles";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::DEL, "/guilds/" + guildId + "/members/" + userId
			+ "/roles/" + roleId);
	statusCheck(r, status_codes::NoContent, "removeGuildMemberRole");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::removeGuildMember(string guildId, string userId) {
	string endpoint = "/guilds/" + guildId + "/members";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::DEL, endpoint + "/" + userId);
	statusCheck(r, status_codes::NoContent, "removeGuildMember");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::createGuildBan(string guildId, string userId) {
	string endpoint = "/guilds/" + guildId + "/bans";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::PUT, endpoint + "/" + userId);
	statusCheck(r, status_codes::NoContent, "createGuildBan");
	rateLimiter.report(endpoint, r.headers());
}

void DiscordAPI::removeGuildBan(string guildId, string userId) {
	string endpoint = "/guilds/" + guildId + "/bans";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::DEL, endpoint + "/" + userId);
	statusCheck(r, status_codes::NoContent, "removeGuildBan");
	rateLimiter.report(endpoint, r.headers());
}

Message DiscordAPI::getChannelMessage(string channelId, string messageId) {
	string endpoint = "/channels/" + channelId + "/messages";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::GET, endpoint + "/" + messageId);
	statusCheck(r, status_codes::OK, "");
	rateLimiter.report(endpoint, r.headers());
    json::value j = r.extract_json().get();
	return j;
}

void DiscordAPI::triggerTyping(string channelId) {
	string endpoint = "/channels/" + channelId + "/typing";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::POST, endpoint);
	statusCheck(r, status_codes::NoContent, "triggerTyping");
	rateLimiter.report(endpoint, r.headers());
}

vector<Channel> DiscordAPI::getUserDMs() {
	string endpoint = "/users/@me/channels";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::GET, endpoint);
	statusCheck(r, status_codes::OK, "getUserDMs");
	rateLimiter.report(endpoint, r.headers());
	vector<Channel> res;
	json::value v = r.extract_json().get();
	if(!v.is_array())
		throw runtime_error("getUserDMs : returned is not an array !");
	for(json::value& j : v.as_array())
		res.emplace_back(j);
	return res;
}

Channel DiscordAPI::createDM(string recipientId) {
	json::value payload;
	payload["recipient_id"] = json::value(recipientId);
	string endpoint = "/users/@me/channels";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::POST, endpoint, payload);
	statusCheck(r, status_codes::OK, "createDM");
	rateLimiter.report(endpoint, r.headers());
	return r.extract_json().get();
}

User DiscordAPI::getUser(string userId) {
	string endpoint = "/users";
	rateLimiter.ask(endpoint);
	http_response r = httpRequest(methods::GET, endpoint + "/" + userId);
	statusCheck(r, status_codes::OK, "getUser");
	rateLimiter.report(endpoint, r.headers());
	return r.extract_json().get();
}
