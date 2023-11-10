#include <iostream>
#include "../inc/Server.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
        return (std::cout << "Wrong arguments" << std::endl, 1);
    
	std::string argv_str = argv[1]; 
    if (argv_str.size() == 0 || argv_str.size() > 10)
        return (std::cout << "Wrong arguments" << std::endl, 1);

    Server server(atoi(argv[1]), argv[2]);

    try
    {
        server.openSocket();
        server.init();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
	server.run();

}