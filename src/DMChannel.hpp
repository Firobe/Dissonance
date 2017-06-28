#ifndef DMCHANNEL_HPP
#define DMCHANNEL_HPP

#include <string>
#include <cpprest/json.h>

#include "User.hpp"

using namespace web;

struct DMChannel {
	std::string id;
	User recipient;
	bool is_private;
	std::string last_message_id;

	DMChannel() = default;
	DMChannel(json::value);
};

#endif
