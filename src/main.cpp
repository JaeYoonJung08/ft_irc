#include <iostream>
#include "../inc/Server.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
        return 1;
	Server server(stoi(argv[1]), argv[2]);

	server.openSocket();
	server.init();

	server.run();

}