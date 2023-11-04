#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include "Message.hpp"

#define IN_BUFFER_SIZE 1024

class Client
{
  private:
    int socket;
    std::string nickname;
    std::string username;
    std::string inBuffer;

  public:
    Client();
    Client(int socket);
    ~Client();
    // Client(int socket, const std::string &nickname, const std::string &username);
    void setUsername(std::string username);
    void setNickname(std::string nickname);
    const std::string &getNickname() const;

    std::vector<Message> readData();
    std::vector<Message> extractMessageFromBuffer();
};

#endif