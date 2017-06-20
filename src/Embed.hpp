#ifndef EMBED_HPP
#define EMBED_HPP

#include <vector>
#include <string>
#include <cpprest/json.h>

using namespace utility;
using namespace web;

struct Image {
	std::string url;
	std::string proxy_url;
	int width, height;
	Image(std::string, std::string, int, int);
	Image() = default;
	json::value toJson();
	bool empty();
};

struct Provider {
	std::string name, url;
	Provider(std::string, std::string);
	Provider() = default;
	json::value toJson();
	bool empty();
};

struct Author {
	std::string name, url, icon_url, proxy_icon_url;
	Author(std::string, std::string, std::string, std::string);
	Author() = default;
	json::value toJson();
	bool empty();
};

struct Footer {
	std::string text, icon_url, proxy_icon_url;
	Footer(std::string, std::string, std::string);
	json::value toJson();
	Footer() = default;
	bool empty();
};

struct Field {
	std::string name, value;
	bool Inline;
	Field(std::string, std::string, bool);
	Field() = default;
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
	std::vector<Field> fields;
	Embed() = default;
	Embed(std::string, int = 0x80);
	json::value toJson();
	bool empty();
};

struct Message {
	std::string content;
	Embed embed;
	Message() = default;
	Message(std::string);
	Message(const char*);
	Message(Embed);
	json::value toJson();
};

#endif
