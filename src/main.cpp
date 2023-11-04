#include <iostream>
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
using namespace std;

// Command * Command::commandInstance = NULL;

int main(int argc, char* argv[])
{
	Server server(stoi(argv[1]), argv[2]);

	server.openSocket();
	server.init();

	server.run();

}