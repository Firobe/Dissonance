#include "DMChannel.hpp"
#include "Retrieve.hpp"

using namespace std;

DMChannel::DMChannel(json::value v) :
	recipient(v["recipient"])
{
	RETRIEVE(v, id, string);
	RETRIEVE_BOOL(v, is_private);
	RETRIEVE(v, last_message_id, string);
}
