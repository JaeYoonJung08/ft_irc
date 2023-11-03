#ifndef SERVER_HPP
#define SERVER_HPP

#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <list>
#include <map>

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Message.hpp"
#include "../inc/Command.hpp"

#define LISTEN_BACKLOG_NUM 5 // listen 에서 대기 큐 갯수.

enum Cmd
{
    PASS,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    PART,
    QUIT,
    EXIT,
    KICK,
    INVITE,
    MODE,
    TOPIC
};

class Server
{
  private:
    int portNumber;
    std::string password;
    int serverSocket;
    int kque;

    std::map<std::string, int> nicknameToSocketFd;
    std::map<int, Client> socketFdToClient;
    // TODO: 채널 이름 to Channel map
    std::map<std::string, Channel> channel;

    Server();

  public:
    Server(int portNumber, std::string password);
    ~Server();

/////////////////////////////////////////
    // Command 클래스에 Server 인스턴스를 제공하는 메서드 추가
    // Command& getCommandInstance();
    
    // 새로운 함수를 통해 Server 클래스의 멤버 변수에 대한 액세스 제공
    //int getPortNumber() const { return portNumber;}
    const std::map<std::string, int>& getNicknameToSocketFd() const 
    {
        return nicknameToSocketFd;
    }

    const std::map<std::string, int>& getSocketFdToClient() const 
    {
        return socketFdToClient;
    }

        const std::map<std::string, int>& getChannel() const 
    {
        return channel;
    }

/////////////////////////////////

    void openSocket();
    void init();
    void run();
    void handleNewConnection(int sockFd);
    void handleExistingConnection(int fd, struct kevent event);
    void handleExistingConnection_send_client(int fd, struct kevent event);
    bool isConnected(int fd, struct kevent event);
    void terminateConnection(int fd, struct kevent event);
    void execCommand(Message message);
    bool password_checker(const std::string &str);



};
/////////////////////
// Command& Server::getCommandInstance()
//  {
//     return Command::getInstance(*this); // Command 클래스 생성 시 Server 클래스 인스턴스 전달
// }
///////////////////////
#endif
