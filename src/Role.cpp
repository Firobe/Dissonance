#include "Role.hpp"
#include "Retrieve.hpp"

using namespace std;

Role::Role(json::value v) {
	RETRIEVE(v, id, string);
	RETRIEVE(v, name, string);
	RETRIEVE(v, color, integer);
	RETRIEVE(v, position, integer);
	RETRIEVE(v, permissions, integer);
	RETRIEVE_BOOL(v, mentionable);
}
