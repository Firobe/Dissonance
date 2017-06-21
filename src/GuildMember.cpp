#include "GuildMember.hpp"

using namespace std;

GuildMember::GuildMember(json::value v) :
	user(v["user"])
{
	if(v.has_field("nick"))
		nick = v["nick"].as_string();
	if(v.has_field("jointed_at"))
		joined_at = v["joined_at"].as_string();
	if(v.has_field("mute"))
		mute = v["mute"].as_bool();
	if(v.has_field("dead"))
		deaf = v["deaf"].as_bool();
	for(json::value& r : v["roles"].as_array())
		roles.emplace_back(r);
}