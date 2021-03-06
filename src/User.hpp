#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <cpprest/json.h>

using namespace web;

struct User {
	std::string id;
	std::string username;
	std::string discriminator;
	std::string avatar;
	User() = default;
	User(json::value);
	std::string avatarURL();
};

#endif
