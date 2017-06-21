#ifndef ROLE_HPP

#include <string>
#include <cpprest/json.h>

#include "Embed.hpp"

using namespace utility;
using namespace web;

struct Role {
	std::string id;
	std::string name;
	unsigned color;
	unsigned position;
	unsigned permissions;
	bool mentionable;
	Role() = default;
	Role(json::value);
};

#endif
