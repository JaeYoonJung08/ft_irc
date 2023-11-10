#include <iostream>
#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
using namespace std;

// Command * Command::commandInstance = NULL;

int main(int argc, char* argv[])
{
    if (argc != 3)
        return (std::cout << "Wrong arguments" << std::endl, 0);
    
	Server server(stoi(argv[1]), argv[2]);

	server.openSocket();
	server.init();

	server.run();

}