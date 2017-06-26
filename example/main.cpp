#include <iostream>
#include <stdexcept>
#include <ctime>

#include "bot.hpp"

using namespace std;

int main(int argc, char** argv) {
	if(argc < 2) {
		cerr << "Please provide the Bot Token as a parameter !" << endl;
		return EXIT_FAILURE;
	}
	Bot bot;
	bot.start(argv[1]);
	return EXIT_SUCCESS;
}
