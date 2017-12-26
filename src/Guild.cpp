#include "Guild.hpp"
#include "Retrieve.hpp"

using namespace std;

Guild::Guild(json::value v)
{
	RETRIEVE(v, id, string);
	RETRIEVE(v, splash, string);
	RETRIEVE(v, name, string);
	RETRIEVE(v, icon, string);
	RETRIEVE(v, splash, string);
	RETRIEVE(v, owner_id, string);
	RETRIEVE(v, region, string);
	RETRIEVE(v, verification_level, integer);
	RETRIEVE(v, default_message_notifications, integer);
	RETRIEVE_BOOL(v, embed_enabled);
	RETRIEVE(v, embed_channel_id, string);
	RETRIEVE(v, afk_channel_id, string);
	RETRIEVE(v, afk_timeout, integer);
	RETRIEVE(v, mfa_level, integer);
	RETRIEVE(v, joined_at, string);
	RETRIEVE_BOOL(v, large);
	RETRIEVE_BOOL(v, unavailable);
	RETRIEVE(v, member_count, integer);
    RETRIEVE_ARRAY(v, roles);
    RETRIEVE_ARRAY(v, members);
    RETRIEVE_ARRAY(v, channels);
}
