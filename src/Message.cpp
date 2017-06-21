#include "Message.hpp"
#include "Retrieve.hpp"

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

Message::Message(json::value v) {
	RETRIEVE(v, id, string);
	RETRIEVE(v, channel_id, string);
	RETRIEVE(v, content, string);
	RETRIEVE(v, timestamp, string);

	if(v.has_field("author"))
		author = v["author"];

	//TODO embed parsing
	if(v.has_field("mentions"))
		for(json::value& j : v["mentions"].as_array())
			mentions.emplace_back(j);
	if(v.has_field("mention_roles"))
		for(json::value& j : v["mention_roles"].as_array())
			mention_roles.emplace_back(j);
}
