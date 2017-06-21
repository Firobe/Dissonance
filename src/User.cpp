#include "User.hpp"
#include "Retrieve.hpp"

using namespace std;

User::User(json::value v) {
	RETRIEVE(v, id, string);
	RETRIEVE(v, username, string);
	RETRIEVE(v, discriminator, string);
}
