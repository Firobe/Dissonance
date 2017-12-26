#include "Channel.hpp"
#include "Retrieve.hpp"

using namespace std;

Channel::Channel(json::value v) {
    name = "DM";
	RETRIEVE(v, id, string);
	RETRIEVE(v, name, string);
	RETRIEVE(v, type, integer);
	RETRIEVE(v, position, integer);
	RETRIEVE(v, guild_id, string);
	RETRIEVE(v, last_message_id, string);
    RETRIEVE_ARRAY(v, recipients);
}

