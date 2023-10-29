#include <iostream>
#include "../inc/Server.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	Server server(stoi(argv[1]), argv[2]);

	server.openSocket();
	server.init();

	server.run();
}