#ifndef EMBED_HPP
#define EMBED_HPP

#include <string>
#include <cpprest/json.h>

using namespace utility;
using namespace web;

struct Image {
	std::string url;
	std::string proxy_url;
	int width, height;
	Image(std::string, std::string, int, int);
    Image(json::value);
	Image() = default;
	json::value toJson();
	bool empty();
};

struct Provider {
	std::string name, url;
	Provider(std::string, std::string);
    Provider(json::value);
	Provider() = default;
	json::value toJson();
	bool empty();
};

struct Author {
	std::string name, url, icon_url, proxy_icon_url;
	Author(std::string, std::string, std::string, std::string);
    Author(json::value);
	Author() = default;
	json::value toJson();
	bool empty();
};

struct Footer {
	std::string text, icon_url, proxy_icon_url;
	Footer(std::string, std::string, std::string);
    Footer(json::value);
	json::value toJson();
	Footer() = default;
	bool empty();
};

struct Field {
	std::string name, value;
	bool Inline;
	Field(std::string, std::string, bool);
    Field(json::value);
	Field() = default;
	json::value toJson();
	bool empty();
};

struct Video {
	std::string url;
    int width, height;
    Video(std::string, int, int);
    Video(json::value);
	Video() = default;
	json::value toJson();
	bool empty();
};

struct Embed {
	std::string title;
	std::string description;
	std::string url;
	int color;
	std::string timestamp;
	Footer footer;
	Image image;
	Image thumbnail;
	Provider provider;
	Author author;
    Video video;
	std::vector<Field> fields;
	Embed() = default;
	Embed(std::string, int = 0x80);
    Embed(json::value);
	json::value toJson();
	bool empty();
};

#endif
