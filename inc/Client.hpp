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
    std::string ipaddress;
    std::string inBuffer;
    std::string outBuffer;


  public:
    static void setServerPtr(Server *server_ptr);

    Client();
    Client(int socket);
    ~Client();
    void setUsername(std::string username);
    void setNickname(std::string nickname);
    void setIpaddress(std::string ip_address);
    std::string &getNickname();
    std::string &getUsername();
    std::string &getIpaddress();
    int getSocket() const;

    std::vector<Message> readData();
    std::vector<Message> extractMessageFromBuffer();
    void sendMessage(Message &message);
    void sendMessage(std::string &string);

    void writeOn();
    void writeOff();

    void sendData();
};

#endif