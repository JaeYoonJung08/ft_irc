#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

class Message
{
  private:
    int socket;
    std::string prefix;
    std::string command;
    std::vector<std::string> argument;

  public:
    Message(int socket, std::string msg);
    Message(int socket,
            const std::string &prefix,
            const std::string &command,
            const std::vector<std::string> &argument);
    ~Message();

    const std::string &getPrefix() const;
    const std::string &getCommand() const;
    std::vector<std::string> &getArg();
    void setPrefix(const std::string &prefix);
    int getSocket();
    void parse(std::string msg);
};

std::vector<std::string> split(std::string input, char delimiter);

#endif