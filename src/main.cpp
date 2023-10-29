#include <iostream>
#include "../inc/IrcServ.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	IrcServ ircServ(stoi(argv[1]), argv[2]);

	ircServ.openSocket();
	ircServ.init();

	ircServ.run();
}