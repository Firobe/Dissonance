#include "Guild.hpp"

using namespace std;

Guild::Guild(json::value v) :
	id(v["id"].as_string()),
	name(v["name"].as_string()),
	icon(v["icon"].as_string()),
	owner_id(v["owner_id"].as_string()),
	region(v["region"].as_string()),
	afk_channel_id(v["afk_channel_id"].as_string()),
	afk_timeout(v["afk_timeout"].as_integer()),
	embed_enabled(v["embed_enabled"].as_bool()),
	embed_channel_id(v["embed_channel_id"].as_string()),
	verification_level(v["verification_level"].as_integer()),
	default_message_notifications(v["default_message_notifications"]
			.as_integer())
{
	if(v.has_field("splash") and !v["splash"].is_null())
		splash = v["splash"].as_string();
	for(json::value& j : v["roles"].as_array())
		roles.emplace_back(j);
	if(v.has_field("mfa_level"))
		mfa_level = v["mfa_level"].as_integer();
	if(v.has_field("joined_at"))
		joined_at = v["joined_at"].as_string();
	if(v.has_field("large"))
		large = v["large"].as_bool();
	if(v.has_field("unavailable"))
		unavailable = v["unavailable"].as_bool();
	if(v.has_field("member_count")) 
		member_count = v["member_count"].as_integer();
	if(v.has_field("members"))
		for(json::value& j : v["members"].as_array())
			members.emplace_back(j);
	if(v.has_field("channels"))
		for(json::value& j : v["channels"].as_array())
			channels.emplace_back(j);
}
