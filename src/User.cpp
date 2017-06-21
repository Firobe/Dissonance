#include "User.hpp"

using namespace std;

User::User(json::value v) :
	id(v["id"].as_string()),
	username(v["username"].as_string()),
	discriminator(v["discriminator"].as_string())
{}
