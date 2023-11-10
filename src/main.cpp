#include <iostream>
#include "../inc/Server.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
        return (std::cout << "Wrong arguments" << std::endl, 0);
    
	std::string argv_str = argv[1]; 
    if (argv_str.size() == 0 || argv_str.size() > 10)
        return (std::cout << "Wrong arguments" << std::endl, 0);

    Server server(atoi(argv[1]), argv[2]);

	server.openSocket();
	server.init();

	server.run();

}