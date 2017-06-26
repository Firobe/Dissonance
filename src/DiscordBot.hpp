#ifndef DISCORDBOT_HPP
#define DISCORDBOT_HPP

#include "Guild.hpp"
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
    virtual void onMessageCreate(Message) {}
    virtual void onMessageDelete(Message) {}
    virtual void onGuildCreate(Guild) {}
	virtual void onChannelCreate(Channel) {}
	virtual void onChannelUpdate(Channel) {}
	virtual void onChannelDelete(Channel) {}
	virtual void onGuildMemberAdd(std::string, GuildMember) {}
	virtual void onGuildMemberRemove(std::string, User) {}
	virtual void onGuildMemberUpdate(std::string, GuildMember) {}
	virtual void onGuildRoleCreate(std::string, Role) {}
	virtual void onGuildRoleUpdate(std::string, Role) {}
	virtual void onGuildRoleDelete(std::string, std::string) {}
	virtual void onGuildBanAdd(std::string, User) {}
	virtual void onGuildBanRemove(std::string, User) {}
};

#endif
