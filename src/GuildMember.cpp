#include "GuildMember.hpp"
#include "Retrieve.hpp"

using namespace std;

GuildMember::GuildMember(json::value v) :
	user(v["user"])
{
	RETRIEVE(v, nick, string);
	RETRIEVE(v, joined_at, string);
	RETRIEVE_BOOL(v, mute);
	RETRIEVE_BOOL(v, deaf);
	if(v.has_field("roles") and v["roles"].is_array())
		for(json::value& r : v["roles"].as_array())
			roles.push_back(r.as_string());
}
