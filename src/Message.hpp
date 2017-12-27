#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>
#include <cpprest/json.h>

#include "Embed.hpp"
#include "Role.hpp"
#include "User.hpp"

using namespace web;

struct Attachment {
    std::string id;
    std::string filename;
    int size;
    std::string url;
    std::string proxy_url;
    int height;
    int width;

    json::value toJson();
    //Used for parsing received Attachments
    Attachment(json::value);
};

struct Message {
	std::string id;
	std::string channel_id;
	std::string content;
	std::string timestamp;
	User author;
    std::vector<Attachment> attachments;
	std::vector<User> mentions;
	std::vector<Role> mention_roles;
	Embed embed;

	Message() = default;
	//Used for creating messages to send
	Message(std::string);
	Message(const char*);
	Message(Embed);
	json::value toJson();

	//Used for parsing received messages
	Message(json::value);
};

#endif
