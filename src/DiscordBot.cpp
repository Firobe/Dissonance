#include <iostream>

#include "DiscordBot.hpp"

using namespace std;

DiscordBot::DiscordBot() :
	api(nullptr) {}

DiscordBot::~DiscordBot() {
	delete api;
}

void DiscordBot::start(const std::string& token) {
	api = new DiscordAPI();
	int lastRetry = 0;
	for(;;){
		try {
			api->connect(this, token);
			api->wait();
		}
		catch (const exception &e) {
			cerr << "ERREUR GRAVE : " << e.what() << endl;
		}

		if(time(nullptr) - lastRetry < 20) {
			cerr << "Retrials are too close !! COMPLETELY EXITING !!" << endl;
			return;
		}
		cerr << "Will try to resume connection !" << endl;
		sleep(3);
		lastRetry = time(nullptr);
	}
}
