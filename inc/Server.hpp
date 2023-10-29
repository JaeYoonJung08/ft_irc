#ifndef IRCSERV_HPP
#define IRCSERV_HPP

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
#define LISTEN_BACKLOG_NUM 5 // listen 에서 대기 큐 갯수.

class IrcServ
{
  private:
    int portNumber;
    std::string password;
    int serverSocket;
    int kque;

    IrcServ();

  public:
    IrcServ(int portNumber, std::string password);
    ~IrcServ();

    void openSocket();
    void init();
    void run();
    void handleNewConnection(int sockFd);
    void handleExistingConnection(int fd);
};

#endif
