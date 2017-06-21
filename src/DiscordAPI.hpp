#ifndef API_HPP
#define API_HPP

#include <thread>
#include <string>
#include <chrono>

#include <cpprest/http_client.h>
#include <cpprest/ws_client.h>
#include <cpprest/json.h>

#include "Message.hpp"

using namespace utility;                    // Common utilities like std::string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace web::websockets::client;    // Websocket client


class DiscordBot;

http_response statusCheck(http_response);

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
    json::value _user;
    DiscordBot* _bot;
	std::chrono::high_resolution_clock::time_point _startTime;
public:
    void connect(DiscordBot* bot, std::string token);
	void wait();
	DiscordAPI();
	~DiscordAPI();
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
    json::value httpRequest(const method& method, std::string endpoint, json::value body = json::value());
    //PUBLIC API
public:
	//Channel
    void sendMessage(Message content, std::string channel);
	json::value getChannelMessage(std::string channelId, std::string messageId);
	void deleteMessage(std::string channelId, std::string messageId);
	void bulkDeleteMessages(std::string channelId, std::vector<std::string>& messageIds);
	void triggerTyping(std::string channelId);
	//Guild
    void addGuildMemberRole(std::string guildId, std::string userId, std::string roleId);
    void removeGuildMemberRole(std::string guildId, std::string userId, std::string roleId);
	void removeGuildMember(std::string guildId, std::string userId);
	void createGuildBan(std::string guildId, std::string userId);
	void removeGuildBan(std::string guildId, std::string userId);
    const json::value& getUser();
	std::chrono::minutes upTime();
};

#include "DiscordBot.hpp"

#endif
