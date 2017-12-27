#include "Message.hpp"
#include "Retrieve.hpp"

using namespace std;

Attachment::Attachment(json::value v) {
    RETRIEVE(v, id, string);
    RETRIEVE(v, filename, string);
    RETRIEVE(v, size, integer);
    RETRIEVE(v, url, string);
    RETRIEVE(v, proxy_url, string);
    RETRIEVE(v, height, integer);
    RETRIEVE(v, width, integer);
}

json::value Attachment::toJson() {
    json::value r;
    TOJSON(r, id);
    TOJSON(r, filename);
    TOJSON_NOCHECK(r, size);
    TOJSON(r, url);
    TOJSON(r, proxy_url);
    TOJSON_NOCHECK(r, height);
    TOJSON_NOCHECK(r, width);
    return r;
}

Message::Message(string s) : content(s) {}
Message::Message(Embed e) : embed(e) {}
Message::Message(const char* s) : Message(string(s)) {}

json::value Message::toJson() {
	json::value r;
    TOJSON(r, content);
    TOJSON_REC(r, embed);

    if(!attachments.empty()) {
        json::value att;
        for(unsigned i = 0 ; i < attachments.size() ; ++i)
            att[i] = attachments[i].toJson();
        r["attachments"] = att;
    }
	return r;
}

Message::Message(json::value v) {
	RETRIEVE(v, id, string);
	RETRIEVE(v, channel_id, string);
	RETRIEVE(v, content, string);
	RETRIEVE(v, timestamp, string);
    RETRIEVE_REC(v, author);
    RETRIEVE_REC(v, embed);
    RETRIEVE_ARRAY(v, mentions);
    RETRIEVE_ARRAY(v, mention_roles);
    RETRIEVE_ARRAY(v, attachments);
}
