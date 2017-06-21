#include "GuildMember.hpp"

using namespace std;

GuildMember::GuildMember(json::value v) :
	user(v["user"])
{
	if(v.has_field("nick") and !v["nick"].is_null())
		nick = v["nick"].as_string();
	if(v.has_field("joined_at"))
		joined_at = v["joined_at"].as_string();
	if(v.has_field("mute"))
		mute = v["mute"].as_bool();
	if(v.has_field("dead"))
		deaf = v["deaf"].as_bool();
	for(json::value& r : v["roles"].as_array())
		roles.push_back(r.as_string());
}
