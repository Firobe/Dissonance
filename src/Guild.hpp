#ifndef GUILD_HPP
#define GUILD_HPP

#include <vector>
#include <string>
#include <cpprest/json.h>

#include "Role.hpp"
#include "User.hpp"
#include "GuildMember.hpp"
#include "Channel.hpp"

using namespace web;

struct Guild {
	std::string id;
	std::string name;
	std::string icon;
	std::string splash;
	std::string owner_id;
	std::string region;
	std::string afk_channel_id;
	int afk_timeout;
	bool embed_enabled;
	std::string embed_channel_id;
	int verification_level;
	int default_message_notifications;
	std::vector<Role> roles;
	//Extra
	int mfa_level;
	std::string joined_at;
	bool large;
	bool unavailable;
	int member_count;
	std::vector<GuildMember> members;
	std::vector<Channel> channels;

	Guild() = default;
	Guild(json::value);
};

#endif
