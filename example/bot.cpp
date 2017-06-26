#include "bot.hpp"

using namespace std;

/**
 * Upon messon reception
 */
void Bot::onMessageCreate(Message message) {
	//If the user types "Hello !", answer "Hello @user !"
	if (message.content == "Hello !")
		api->sendMessage("Hello <@" + message.author.id + "> !", message.channel_id);
}

/**
 * Upon connection to a new server
 */
void Bot::onGuildCreate(Guild guild) {
	cout << "Connection to a new guild called '" << guild.name << "' !" << endl;
	cout << "Found " << guild.channels.size() << " channels." << endl;
	cout << "Found " << guild.roles.size() << " roles." << endl;
	cout << "Found " << guild.members.size() << " members." << endl;
}

/**
 * Upon member join
 */
void Bot::onGuildMemberAdd(string guildId, GuildMember member) {
	cout << "The user " << member.user.username << " juste joined the guild n°" << guildId << endl;
}
