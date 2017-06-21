#include "Channel.hpp"
#include "Retrieve.hpp"

using namespace std;

Channel::Channel(json::value v) {
	RETRIEVE(v, id, string);
	RETRIEVE(v, name, string);
	RETRIEVE(v, type, string);
	RETRIEVE(v, position, integer);
	RETRIEVE(v, guild_id, string);
	RETRIEVE_BOOL(v, is_private);
	RETRIEVE(v, last_message_id, string);
}

