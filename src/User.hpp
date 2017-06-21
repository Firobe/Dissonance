#ifndef USER_HPP

#include <string>
#include <cpprest/json.h>

#include "Embed.hpp"

using namespace utility;
using namespace web;

struct User {
	std::string id;
	std::string username;
	std::string discriminator;
	User() = default;
	User(json::value);
};

#endif
