#include "Channel.hpp"

using namespace std;
/*
 *
	std::string id;
	std::string guild_id;
	std::string name;
	std::string type;
	int position;
	bool is_private;
	std::string last_message_id;
	*/

Channel::Channel(json::value v) :
	id(v["id"].as_string()),
	guild_id(v["guild_id"].as_string()),
	name(v["name"].as_string()),
	type(v["type"].as_string()),
	position(v["position"].as_integer())
{
	if(v.has_field("is_private"))
		is_private = v["is_private"].as_bool();
	if(v.has_field("last_message_id"))
		last_message_id = v["last_message_id"].as_string();
}
