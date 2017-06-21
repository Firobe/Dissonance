#ifndef ROLE_HPP
#define ROLE_HPP

#include <string>
#include <cpprest/json.h>

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
