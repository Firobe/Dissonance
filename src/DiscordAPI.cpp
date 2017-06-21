#include <condition_variable>

#include "DiscordAPI.hpp"

using namespace std;
mutex mtx;
condition_variable cv;
bool stop = false;

http_response statusCheck(http_response r) {
    if (r.status_code() == status_codes::BadRequest or
			r.status_code() == status_codes::Forbidden or
			r.status_code() == status_codes::Unauthorized or
			r.status_code() == status_codes::NotFound or
			r.status_code() == status_codes::MethodNotAllowed or
			r.status_code() == 429 or
			r.status_code() >= 500)
        throw runtime_error("Received status code " + to_string(r.status_code()));

    return r;
}

DiscordAPI::DiscordAPI() :
    _http_client("https://discordapp.com/api/"),
	_bot(nullptr) {
		_startTime = chrono::high_resolution_clock::now();
}

DiscordAPI::~DiscordAPI() {
	cerr << "Destructor appelé !" << endl;
}

void DiscordAPI::wait() {
	unique_lock<mutex> lck(mtx);
	while(!stop) cv.wait(lck);
	cerr << "API IS STOPPING !" << endl;
	_web_client.close(websocket_close_status::going_away, "Manual close").wait();
	cerr << "API IS STOPPED !" << endl;
}

void DiscordAPI::connect(DiscordBot* bot, string token) {
	stop = false;
	_dead = false;
	_initPhase = 0;
	_web_client = websocket_callback_client();
	_resuming = (_bot != nullptr);
	_bot = bot;
	_token = token;
    cout << "DISCORD++ INIT\n============================================" << endl;
    //GET endpoint
    _wsEndpoint = httpRequest(methods::GET, "/gateway")["url"].as_string();
    cout << "Getting gateway endpoint..." << endl;
    cout << "Gateway endpoint : " << _wsEndpoint << endl;
    //Connection
    uri_builder builder(_wsEndpoint);
    builder.append_query("v", 5);
    builder.append_query("encoding", "json");
	_web_client.set_message_handler(bind(&DiscordAPI::receiveAndDispatch, this, placeholders::_1));

	//Close manager
	_web_client.set_close_handler([](websocket_close_status closeStatus, string reason, error_code err) {
			cerr << "CONNECTION WAS CLOSED : " << reason << endl;
			cerr << "CLOSE STATUS : " << (unsigned) closeStatus << endl;
			cerr << "ERROR_CODE " << err << endl;
			stop = true;
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
	cout << "Okay !\nSending cool presence message...";
	json::value payload = json::value::parse("{\"game\":{\"name\":\"dormir...\"},\"idle_since\":null}");
	sendJson(gatewayPayload(payload, 3));

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
        cout << "Exception keepAlive : " << e.what() << endl;
		stop = true;
		cv.notify_all();
    }
	cerr << "End of keepAlive !" << endl;
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

		default:
			cout << "Received unprocessed message " << received.serialize() << endl;
			cout << "OP was " << received["op"].as_integer() << endl;
		}
	}
	} catch(exception& e) {
        cout << "Exception receiveAndDispatch : " << e.what() << endl;
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
			_bot->onGuildBanAdd(event["guild_id"].as_string(), event);
		else if(name == "GUILD_BAN_REMOVE")
			_bot->onGuildBanRemove(event["guild_id"].as_string(), event);
	}
	catch(exception& e) {
		cout << "Exception while processing event (" << name << ") : " << e.what() << endl;
		throw e;
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

const json::value& DiscordAPI::getUser() {
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

json::value DiscordAPI::httpRequest(const method& method, string endpoint, json::value body) {
    http_request request(method);
    request.set_request_uri(endpoint);
    request.headers().add("Authorization", "Bot " + _token);
    request.headers().add("User-Agent", "DiscordBot (Sakamoto, 6.9)");
	if(method != methods::GET) {
		request.headers().add("Content-Length", body.serialize().size());
		request.set_body(body);
	}
	http_response response = _http_client.request(request).get();
	statusCheck(response);
	http_headers headers = response.headers();

	//TODO SMARTER RATE LIMITING
	if(headers.has("X-RateLimit-Limit")){
		string rem = headers["X-RateLimit-Remaining"];
		string res = headers["X-RateLimit-Reset"];
		if(rem == "0"){
			time_t t = stoul(res);
			int toSleep = t - time(NULL);
			cout << "WILL SLEEP " << toSleep << " SECONDS FOR RATE LIMITING AFTER " << endpoint << endl;
			this_thread::sleep_for(chrono::seconds(toSleep));
		}
	}
	

	if(response.status_code() == status_codes::OK){
		try{
		return response.extract_json().get();
		}catch(...){ return json::value("NULL"); }
	}
	else return json::value("NULL");
}

/******************************************************
 *     API BEGINS HERE
 ******************************************************/


void DiscordAPI::sendMessage(Message me, string channel) {
    json::value m = me.toJson();
	try{
    httpRequest(methods::POST, "/channels/" + channel + "/messages", m);
	}catch(exception& e){ cerr << "Sending message : " << e.what() << endl;}
}

void DiscordAPI::deleteMessage(string channelId, string messageId) {
	httpRequest(methods::DEL, "/channels/" + channelId + "/messages/" + messageId);
}

void DiscordAPI::bulkDeleteMessages(string channelId, vector<string>& messageIds) {
	json::value payload;
	json::value arr;
	for(unsigned i = 0 ; i < messageIds.size() ; ++i)
		arr[i] = json::value(messageIds[i]);
	payload["messages"] = arr;
	httpRequest(methods::POST, "/channels/" + channelId + "/messages/bulk-delete", payload);
}

void DiscordAPI::addGuildMemberRole(string guildId, string userId, string roleId) {
	httpRequest(methods::PUT, "/guilds/" + guildId + "/members/" + userId
			+ "/roles/" + roleId);
}

void DiscordAPI::removeGuildMemberRole(string guildId, string userId, string roleId) {
	httpRequest(methods::DEL, "/guilds/" + guildId + "/members/" + userId
			+ "/roles/" + roleId);
}

void DiscordAPI::removeGuildMember(string guildId, string userId) {
	httpRequest(methods::DEL, "/guilds/" + guildId + "/members/" + userId);
}

void DiscordAPI::createGuildBan(string guildId, string userId) {
	httpRequest(methods::PUT, "/guilds/" + guildId + "/bans/" + userId);
}

void DiscordAPI::removeGuildBan(string guildId, string userId) {
	httpRequest(methods::DEL, "/guilds/" + guildId + "/bans/" + userId);
}

json::value DiscordAPI::getChannelMessage(string channelId, string messageId) {
	return httpRequest(methods::GET, "/channels/" + channelId + "/messages/" + messageId);
}

void DiscordAPI::triggerTyping(string channelId) {
	httpRequest(methods::POST, "/channels/" + channelId + "/typing");
}
