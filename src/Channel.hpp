#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <cpprest/json.h>

using namespace web;

struct Channel {
	std::string id;
	std::string guild_id;
	std::string name;
	std::string type;
	int position;
	bool is_private;
	std::string last_message_id;

	Channel() = default;
	Channel(json::value);
};

#endif
