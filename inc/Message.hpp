#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

class Message
{
  private:
    int socket;
    std::string prefix;
    std::string command;
    std::vector<std::string> argument;

  public:
    Message(int socket, std::string msg);
    ~Message();

    std::string getPrefix();
    std::string getCommand();
    std::vector<std::string> getArg();
    int getSocket();

    void parse(std::string msg);
};

#endif