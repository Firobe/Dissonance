#include "GuildMember.hpp"

using namespace std;

GuildMember::GuildMember(json::value v) :
	user(v["user"]),
	joined_at(v["joined_at"].as_string()),
	mute(v["mute"].as_bool()),
	deaf(v["deaf"].as_bool())
{
	if(v.has_field("nick"))
		nick = v["nick"].as_string();
	for(json::value& r : v["roles"].as_array())
		roles.emplace_back(r);
}
