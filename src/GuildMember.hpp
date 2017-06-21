#ifndef GUILDMEMBER_HPP
#define GUILDMEMBER_HPP

#include <string>
#include <vector>
#include <cpprest/json.h>

#include "User.hpp"
#include "Role.hpp"

using namespace web;

struct GuildMember {
	User user;
	std::string nick;
	std::vector<Role> roles;
	std::string joined_at;
	bool mute;
	bool deaf;

	GuildMember() = default;
	GuildMember(json::value);
};

#endif
