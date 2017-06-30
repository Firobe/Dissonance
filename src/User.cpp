#include "User.hpp"
#include "Retrieve.hpp"

using namespace std;

User::User(json::value v) {
	RETRIEVE(v, id, string);
	RETRIEVE(v, username, string);
	RETRIEVE(v, discriminator, string);
	RETRIEVE(v, avatar, string);
}

string User::avatarURL() {
	if(id.empty() or avatar.empty())
		return "https://www.timeshighereducation.com/sites/default/files/byline_photos/default-avatar.png";
	else return "https://cdn.discordapp.com/avatars/"
		+ id + "/" + avatar + ".jpg";
}
