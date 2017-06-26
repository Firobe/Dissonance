#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <Dissonance.hpp>

class Bot : public DiscordBot {
	protected:
		void onMessageCreate(Message message) override;
		void onGuildCreate(Guild) override;
		void onGuildMemberAdd(std::string, GuildMember) override;
};

#endif
