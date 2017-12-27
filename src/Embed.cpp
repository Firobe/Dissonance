#include "Embed.hpp"
#include "Retrieve.hpp"

using namespace std;

Image::Image(string url, string pr, int w, int h) :
	url(url),
	proxy_url(pr),
	width(w),
	height(h) {
}

Image::Image(json::value v) {
    RETRIEVE(v, url, string);
    RETRIEVE(v, proxy_url, string);
    RETRIEVE(v, width, integer);
    RETRIEVE(v, height, integer);
}

json::value Image::toJson() {
	json::value r;
	r["url"] = json::value(url);
	r["proxy_url"] = json::value(proxy_url);
	r["width"] = json::value(width);
	r["height"] = json::value(height);
	return r;
}

bool Image::empty() {
	return url.empty();
}

Provider::Provider(string name, string url) :
	name(name),
	url(url) {}

Provider::Provider(json::value v) {
    RETRIEVE(v, name, string);
    RETRIEVE(v, url, string);
}

json::value Provider::toJson() {
	json::value r;
	r["url"] = json::value(url);
	r["name"] = json::value(name);
	return r;
}

bool Provider::empty() {
	return name.empty();
}

Author::Author(string name, string url, string ic, string pr) :
	name(name),
	url(url),
	icon_url(ic),
	proxy_icon_url(pr) {}

Author::Author(json::value v) {
    RETRIEVE(v, name, string);
    RETRIEVE(v, url, string);
    RETRIEVE(v, icon_url, string);
    RETRIEVE(v, proxy_icon_url, string);
}

json::value Author::toJson() {
	json::value r;
	r["url"] = json::value(url);
	r["name"] = json::value(name);
	r["icon_url"] = json::value(icon_url);
	r["proxy_icon_url"] = json::value(proxy_icon_url);
	return r;
}

bool Author::empty() {
	return name.empty();
}

Video::Video(string url, int w, int h) :
    url(url),
    width(w),
    height(h) {}

Video::Video(json::value v) {
    RETRIEVE(v, url, string);
    RETRIEVE(v, width, integer);
    RETRIEVE(v, height, integer);
}

bool Video::empty() {
    return url.empty();
}

json::value Video::toJson() {
	json::value r;
    TOJSON(r, url);
    TOJSON_NOCHECK(r, width);
    TOJSON_NOCHECK(r, height);
	return r;
}

Footer::Footer(string text, string ic, string pr) :
	text(text),
	icon_url(ic),
	proxy_icon_url(pr) {}

Footer::Footer(json::value v) {
    RETRIEVE(v, text, string);
    RETRIEVE(v, icon_url, string);
    RETRIEVE(v, proxy_icon_url, string);
}

bool Footer::empty() {
	return text.empty();
}

json::value Footer::toJson() {
	json::value r;
	if(!text.empty()) r["text"] = json::value(text);
	if(!icon_url.empty()) r["icon_url"] = json::value(icon_url);
	if(!proxy_icon_url.empty()) r["proxy_icon_url"] = json::value(proxy_icon_url);
	return r;
}

Field::Field(string name, string value, bool Inline) :
	name(name),
	value(value),
	Inline(Inline) {}

Field::Field(json::value v) {
    RETRIEVE(v, name, string);
    RETRIEVE(v, value, string);
    RETRIEVE_BOOL(v, Inline);
}

json::value Field::toJson() {
	json::value r;
	if(value.empty()) value = "[NO TEXT IN MESSAGE]";
	r["name"] = json::value(name);
	r["value"] = json::value(value);
	r["inline"] = json::value(Inline);
	return r;
}

Embed::Embed(string title, int color) :
	title(title),
	color(color) {}

Embed::Embed(json::value v) {
    RETRIEVE(v, title, string);
    RETRIEVE(v, description, string);
    RETRIEVE(v, url, string);
    RETRIEVE(v, color, integer);
    RETRIEVE(v, timestamp, string);
    RETRIEVE_REC(v, footer);
    RETRIEVE_REC(v, image);
    RETRIEVE_REC(v, thumbnail);
    RETRIEVE_REC(v, provider);
    RETRIEVE_REC(v, author);
    RETRIEVE_REC(v, video);
    RETRIEVE_ARRAY(v, fields);
}

json::value Embed::toJson() {
	json::value r;
    TOJSON(r, title);
    TOJSON(r, description);
    TOJSON(r, url);
    TOJSON(r, timestamp);
    TOJSON_NOCHECK(r, color);
    TOJSON_REC(r, footer);
    TOJSON_REC(r, image);
    TOJSON_REC(r, thumbnail);
    TOJSON_REC(r, provider);
    TOJSON_REC(r, author);
    TOJSON_REC(r, video);
	if(fields.size() > 0) {
		json::value a;
		for(unsigned i = 0 ; i < fields.size() ; ++i) a[i] = fields[i].toJson();
		r["fields"] = a;
	}
	return r;
}

bool Embed::empty() {
	return title.empty();
}
