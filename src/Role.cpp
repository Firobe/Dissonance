#include "Role.hpp"

using namespace std;

Role::Role(json::value v) :
	id(v["id"].as_string()),
	name(v["name"].as_string()),
	color(v["color"].as_integer()),
	position(v["position"].as_integer()),
	permissions(v["permissions"].as_integer()),
	mentionable(v["mentionable"].as_bool())
{}
