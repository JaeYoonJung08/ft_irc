#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include "../inc/Message.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

class Command
{
private:
    static Command* commandInstance; // Command 인스턴스의 포인터를 저장하는 정적 멤버 변수
    Server* serverInstance;

public:
    // static Command& getInstance()
    // {
    //     static Command instance;  // 클래스 인스턴스는 한 번만 인스턴스화됨
    //     return instance;
    // }


    // Command 인스턴스를 생성할 때 Server 인스턴스를 전달
    // static Command& getInstance(Server& server)
    // {
    //     static Command instance(server);  // Server 클래스 인스턴스를 받아 Command 클래스 생성
    //     return instance;
    // }

    static Command& getInstance(Server& server);

    // void accessServerMembers() {
    //     // Server 클래스의 멤버 변수에 접근하여 값을 얻어올 수 있음
    //     int port = serverInstance->getPortNumber();
    //     // 다른 멤버 변수들에 대해 동일한 방식으로 get 함수 사용
    // }

    const std::map<std::string,int> &getServernicknameToSocketFd(void); 
    const std::map<int, Client> &getServerSocketFdToClient(void);
    const std::map<std::string, Channel> &getServerChannel(void);
    // {
    //     // Server 클래스의 nicknameToSocketFd 맵에 접근하여 사용 가능
    //     //const std::map<std::string, int>& nickToSocket = serverInstance->getNicknameToSocketFd();
        
    //     return serverInstance->getNicknameToSocketFd();
    //     // 맵 순회 및 작업 수행
    //     // for (const auto& pair : nickToSocket) {
    //     //     std::cout << "닉네임: " << pair.first << ", 소켓: " << pair.second << std::endl;
    //     //     // 닉네임과 해당 소켓에 대한 필요한 작업 수행
    //     // }
    // }

    /* clients */
    void pass(Message &message);
    // void nick(Message &message);
    // void user(Message &message);
    // void privmsg(Message &message);
    // void ping(Message &message);
    // void pong(Message &message);
    // void join(Message &message);
    // int joinChannelNameCheck(std::string name);
    // void part(Message &message);   // 채널 퇴장
    // void quit(std::string reason); // 서버 접속 끊기
    // void exit(void);               // 서버 접속 끊고 프로그램 종료

    /* operators */
    // void kick(Message &message);
    // void topic(Message &message);
    // void invite(Message &message);
    // void mode(Message &message);

    /* command utils */
    //bool setMode(Message &message, Channel channel);

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

Command* Command::commandInstance = NULL;

Command& Command::getInstance(Server& server) 
{
    if (!commandInstance) 
    {
        commandInstance = new Command;
        commandInstance->serverInstance = &server;  // 저장된 Server 인스턴스에 접근
    }
    return *commandInstance;
}
#endif


// #ifndef COMMAND_HPP
// # define COMMAND_HPP

// #include <iostream>
// #include "../inc/Message.hpp"
// #include "../inc/Server.hpp"

// class Command
// {
// private:
//     Command() {}  // 싱글톤 패턴을 위한 프라이빗 생성자
//     Server* serverInstance;
//     Command(Server& server) : serverInstance(&server) {};

// public:
//     // static Command& getInstance()
//     // {
//     //     static Command instance;  // 클래스 인스턴스는 한 번만 인스턴스화됨
//     //     return instance;
//     // }


//     // Command 인스턴스를 생성할 때 Server 인스턴스를 전달
//     static Command& getInstance(Server& server)
//     {
//         static Command instance(server);  // Server 클래스 인스턴스를 받아 Command 클래스 생성
//         return instance;
//     }

//     // void accessServerMembers() {
//     //     // Server 클래스의 멤버 변수에 접근하여 값을 얻어올 수 있음
//     //     int port = serverInstance->getPortNumber();
//     //     // 다른 멤버 변수들에 대해 동일한 방식으로 get 함수 사용
//     // }

//     void accessServerMembers() {
//         // Server 클래스의 nicknameToSocketFd 맵에 접근하여 사용 가능
//         const std::map<std::string, int>& nickToSocket = serverInstance->getNicknameToSocketFd();
//         // 맵 순회 및 작업 수행
//         for (const auto& pair : nickToSocket) {
//             std::cout << "닉네임: " << pair.first << ", 소켓: " << pair.second << std::endl;
//             // 닉네임과 해당 소켓에 대한 필요한 작업 수행
//         }
//     }

//     /* clients */
//     void pass(Message &message);
//     void nick(Message &message);
//     void user(Message &message);
//     void privmsg(Message &message);
//     void ping(Message &message);
//     void pong(Message &message);
//     void join(Message &message);
//     int joinChannelNameCheck(std::string name);
//     void part(Message &message);   // 채널 퇴장
//     void quit(std::string reason); // 서버 접속 끊기
//     void exit(void);               // 서버 접속 끊고 프로그램 종료

//     /* operators */
//     void kick(Message &message);
//     void topic(Message &message);
//     void invite(Message &message);
//     void mode(Message &message);

//     /* command utils */
//     //bool setMode(Message &message, Channel channel);

//     /* error */
//     void state_without_setup_324(Message &message);
//     void password_incorrect_464(Message &message);
//     void command_empty_argument_461(Message &message);

//     /* nick 433, 431 */
//     void	nick_duplicate_check_433(Message &message);
//     void    nick_empty_argument_431(Message &message);

//     /* kick 403 */
//     void no_such_channel_403(Message &message);
//     void no_member_channel_442(Message &message);
//     void user_already_channel_443(Message &message);
//     void no_operator_channel_482(Message &message);
//     void kick_no_users_channel_441(Message &message);

// };

// #endif
