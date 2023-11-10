#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include "Message.hpp"
#include "Server.hpp"

#define IN_BUFFER_SIZE 1024

class Server;

class Client
{
  private:
    static Server *serverPtr;

    int socket;
    std::string nickname;
    std::string username;
    std::string inBuffer;
    std::string outBuffer;
    std::string ipaddress;
    bool isAuthenticated;

  public:
    static void setServerPtr(Server *server_ptr);

    Client();
    Client(int socket);
    ~Client();
    void setUsername(std::string username);
    void setNickname(std::string nickname);
    void setIsAuthenticated(bool auth);

    std::string &getNickname();
    std::string &getUsername();
    int getSocket() const;
    bool &getIsAuthenticated();
    const std::string &getIpaddress() const;
    void setIpaddress(const std::string &ipaddress);

    std::vector<Message> readData();
    std::vector<Message> extractMessageFromBuffer();
    void sendMessage(Message &message);
    void sendMessage(std::string &string);

    void writeOn();
    void writeOff();

    void sendData();

    std::string makePrefix();
};

#endif