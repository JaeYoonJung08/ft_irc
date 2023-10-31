#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
  private:
    int socket;
    std::string nickname;
    std::string username;

  public:
    Client();
    Client(int socket);
    ~Client();
    // Client(int socket, const std::string &nickname, const std::string
    // &username);
    void setUsername(std::string username);
    void setNickname(std::string nickname);
    const std::string &getNickname() const;
};

#endif