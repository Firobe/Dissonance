#include "Embed.hpp"

using namespace std;

Image::Image(string url, string pr, int w, int h) :
	url(url),
	proxy_url(pr),
	width(w),
	height(h) {}

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

Footer::Footer(string text, string ic, string pr) :
	text(text),
	icon_url(ic),
	proxy_icon_url(pr) {}

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

json::value Field::toJson() {
	json::value r;
	r["name"] = json::value(name);
	r["value"] = json::value(value);
	r["Inline"] = json::value(Inline);
	return r;
}

Embed::Embed(string title, int color) :
	title(title),
	color(color) {}

json::value Embed::toJson() {
	json::value r;
	if(!title.empty()) r["title"] = json::value(title);
	if(!description.empty()) r["description"] = json::value(description);
	if(!url.empty()) r["url"] = json::value(url);
	if(!timestamp.empty()) r["timestamp"] = json::value(timestamp);
	if(color != -1) r["color"] = json::value(color);
	if(!footer.empty()) r["footer"] = footer.toJson();
	if(!image.empty()) r["image"] = image.toJson();
	if(!thumbnail.empty()) r["thumbnail"] = thumbnail.toJson();
	if(!provider.empty()) r["provider"] = provider.toJson();
	if(!author.empty()) r["author"] = author.toJson();
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
