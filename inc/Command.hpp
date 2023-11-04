#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "../inc/Message.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

class Server;
class Client;
class Channel;

class Command
{
private:
    static Command commandInstance; // Command 인스턴스의 포인터를 저장하는 정적 멤버 변수
    Server* serverInstance;

public:

    static Command& getInstance(Server& server)
    {
        commandInstance.serverInstance = &server;   
        return commandInstance;
    }

     std::map<std::string,int> &getServernicknameToSocketFd(void); 
     std::map<int, Client> &getServerSocketFdToClient(void);
     std::map<std::string, Channel> &getServerChannel(void);
     std::string &getServerPassWord(void);

    /* clients */
    void pass(Message &message);
    void nick(Message &message);
    void user(Message &message);
    void privmsg(Message &message);
    void ping(Message &message);
    void pong(Message &message);
    void join(Message &message);
    int joinChannelNameCheck(std::string name);
    void part(Message &message);   // 채널 퇴장
    void quit(std::string reason); // 서버 접속 끊기
    void exit(void);               // 서버 접속 끊고 프로그램 종료

    /* operators */
    void kick(Message &message);
    void topic(Message &message);
    void invite(Message &message);
    void mode(Message &message);

    /* command utils */
    bool setMode(Message &message, Channel channel);

    /* error */
    void state_without_setup_324(Message &message);
    void password_incorrect_464(Message &message);
    void command_empty_argument_461(Message &message);

    /* nick 433, 431 */
    void	nick_duplicate_check_433(Message &message);
    void    nick_empty_argument_431(Message &message);

    /* kick 403 */
    void no_such_channel_403(Message &message);
    void no_member_channel_442(Message &message);
    void user_already_channel_443(Message &message);
    void no_operator_channel_482(Message &message);
    void kick_no_users_channel_441(Message &message);

};


#endif

