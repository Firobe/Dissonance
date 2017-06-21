#ifndef BOT_HPP
#define BOT_HPP

#include "DiscordAPI.hpp"
#include "Channel.hpp"
#include "GuildMember.hpp"

class DiscordBot {
	friend DiscordAPI;
public:
    DiscordBot();
	virtual ~DiscordBot();
	virtual void start(const std::string&);
protected:
    DiscordAPI* api;
    virtual void onMessageCreate(Message message) = 0;
    virtual void onMessageDelete(Message message) = 0;
    virtual void onGuildCreate(json::value message) = 0;
	virtual void onChannelCreate(Channel) = 0;
	virtual void onChannelUpdate(Channel) = 0;
	virtual void onChannelDelete(Channel) = 0;
	virtual void onGuildMemberAdd(std::string, GuildMember) = 0;
	virtual void onGuildMemberRemove(std::string, GuildMember) = 0;
	virtual void onGuildMemberUpdate(std::string, GuildMember) = 0;
	virtual void onGuildRoleCreate(std::string, Role) = 0;
	virtual void onGuildRoleUpdate(std::string, Role) = 0;
	virtual void onGuildRoleDelete(std::string, std::string) = 0;
	virtual void onGuildBanAdd(json::value event) = 0;
};

#endif
