#ifndef DISCORDAPI_HPP
#define DISCORDAPI_HPP

#include <thread>
#include <string>
#include <chrono>

#include <cpprest/http_client.h>
#include <cpprest/ws_client.h>
#include <cpprest/json.h>

#include "Message.hpp"
#include "RateLimiter.hpp"
#include "Channel.hpp"
#include "User.hpp"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::websockets::client;

class DiscordBot;

const http_response& statusCheck(const http_response&, status_code, std::string);

class DiscordAPI {
    std::string _token;
	bool _resuming;
    http_client _http_client;
    websocket_callback_client _web_client;
    std::string _wsEndpoint;
    unsigned _seq;
    std::string _sessionID;
    int _heartbeat;
	int _initPhase;
	std::thread _keepAliveTask;
	std::thread _listener;
    bool _dead;
    User _user;
    DiscordBot* _bot;
	std::chrono::high_resolution_clock::time_point _startTime;
	bool _closed;
	RateLimiter rateLimiter;
public:
    void connect(DiscordBot* bot, std::string token);
	void wait();
	DiscordAPI();
    //INTERNAL
private:
	void helloHandler(json::value& ready);
	void readyHandler(json::value& ready);
	void finalHandler();
    void keepAlive();
    json::value identifyValue();
    void eventDispatcher(std::string name, json::value event);
    json::value gatewayPayload(json::value d, int opcode);
    void receiveAndDispatch(const websocket_incoming_message&);

    void sendJson(json::value v);
    http_response httpRequest(const method& method, std::string endpoint, json::value body = json::value());
    //PUBLIC API
public:
	//Channel
    void sendMessage(Message content, std::string channel);
	Message getChannelMessage(std::string channelId, std::string messageId);
	void deleteMessage(std::string channelId, std::string messageId);
	void bulkDeleteMessages(std::string channelId, std::vector<std::string>& messageIds);
	void triggerTyping(std::string channelId);
	//Guild
    void addGuildMemberRole(std::string guildId, std::string userId, std::string roleId);
    void removeGuildMemberRole(std::string guildId, std::string userId, std::string roleId);
	void removeGuildMember(std::string guildId, std::string userId);
	void createGuildBan(std::string guildId, std::string userId);
	void removeGuildBan(std::string guildId, std::string userId);
	std::vector<Channel> getUserDMs();
	Channel createDM(std::string);
	User getUser(std::string);

    User getUser();
	std::chrono::minutes upTime();
};

#include "DiscordBot.hpp"

#endif
