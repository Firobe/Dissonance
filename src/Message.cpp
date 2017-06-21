#include "Message.hpp"

using namespace std;

Message::Message(string s) : content(s) {}
Message::Message(Embed e) : embed(e) {}
Message::Message(const char* s) : Message(string(s)) {}

json::value Message::toJson() {
	json::value r;
	r["content"] = json::value(content);
	if(!embed.empty()) r["embed"] = embed.toJson();
	return r;
}

Message::Message(json::value v) :
	id(v["id"].as_string()),
	channel_id(v["channel_id"].as_string()),
	content(v["content"].as_string()),
	timestamp(v["timestamp"].as_string()),
	author(v["author"])
{
	//TODO embed parsing
	for(json::value& j : v["mentions"].as_array())
		mentions.emplace_back(j);
	for(json::value& j : v["mention_roles"].as_array())
		mention_roles.emplace_back(j);
}
