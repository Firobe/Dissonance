#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <cpprest/json.h>

#include "User.hpp"

using namespace web;

struct Channel {
	std::string id;
	std::string guild_id;
	std::string name;
	int type;
	int position;
	std::string last_message_id;
    std::vector<User> recipients;

	Channel() = default;
	Channel(json::value);
};

#endif
