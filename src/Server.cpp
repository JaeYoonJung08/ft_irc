#include "../inc/Server.hpp"

Server::Server() {}

bool Server::password_checker(const std::string &str)
{
    if (str.empty())
    {
        return (true);
    }
    /* 공백 문자 또는 출력 가능하지 않은 문자 포함하는지 확인.*/
    for (int i = 0; i < str.length(); i++)
    {
        if ((std::isspace(str[i])) || !std::isprint(str[i]))
            return (true);
    }
    return (false);
}

Server::Server(int portNumber, std::string password)
{
    this->portNumber = portNumber;
    this->password = password;

    if (password_checker(this->password))
    {
        std::cerr << "input port invaild" << std::endl;
        std::exit(1);
    }
    this->kque = kqueue(); // kque 생성
    if (this->kque == -1)
        throw std::runtime_error("kqueue error");
}

void Server::openSocket()
{
    sockaddr_in serverAddr;

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP socket 생성
    if (this->serverSocket == -1)
        throw std::runtime_error("socket error");

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr =
        htonl(INADDR_ANY); // 임시로 일단은 모든 주소 받음
    serverAddr.sin_port = htons(this->portNumber);

    // port 재사용시 bind error 없애는 옵션 추가.
    int option = 1;
    setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(option));

    // bind 를 통해 socket이랑 각 설정이랑 연결
    if (bind(this->serverSocket,
             reinterpret_cast<const sockaddr *>(&serverAddr),
             sizeof(serverAddr)) == -1)
        throw std::runtime_error("bind error");
    listen(this->serverSocket, LISTEN_BACKLOG_NUM); // 수신하도록 listen
                                                    // tcp 연결 완료.
}

void Server::init()
{
    struct kevent event;

    EV_SET(&event, this->serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(this->kque, &event, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("kevent attach error");
}

Server::~Server()
{
    close(serverSocket);
    close(kque);
}

void Server::run()
{
    while (true)
    {
        struct kevent events[32];
        int triggered = kevent(this->kque, NULL, 0, events, 32, NULL);
        if (triggered == -1)
            throw std::runtime_error("kevent event error");
        for (int i = 0; i < triggered; i++)
        {
            if ((int)events[i].ident ==
                this->serverSocket) // 소켓에서 이벤트 발생
            {
                std::cout << "새로운 접속!" << std::endl;
                // 새로운 접속 처리
                handleNewConnection(events[i].ident);
            }
            else
            { // 기존 접속
                std::cout << "기존 접속!" << std::endl;
                // naki님이 말씀하신 서버에서 클라이언트로 우째 보내노 라는
                // 질문을 여기서 해결할 것임 kevent 구조체 안에는 filter라는 게
                // 있음 이 친구들을 통해 서버에서 소켓을 받는지, 클라이언트한테
                // 보내줄 것인지를 결정
                if (events[i].filter == EVFILT_READ)
                {
                    // 기존 handleExistingConnection
                    // 여기 이 부분이 클라이언트가 서버로 패킷을 보낼 경우

                    // 여기서 이제해야할 거 개행을 찾지 못한 경우와 개행을 찾은
                    // 경우을 나누어야함. 함수 수정해주자
                    handleExistingConnection(events[i].ident, events[i]);
                }
                // else if (events[i].filter == EVFILT_WRITE)
                // {
                //     //새롭게 추가 된 거
                //     //서버가 클라리언트로 패킷을 보낼 경우
                //     handleExistingConnection_send_client(events[i].ident,
                //     events[i]);
                // }
            }
        }
    }
}

void Server::handleNewConnection(int sockFd)
{
    struct sockaddr client_addr;
    struct kevent event;
    int client_addr_size = sizeof(client_addr);

    int newFd = accept(sockFd, (sockaddr *)&client_addr,
                       (socklen_t *)&client_addr_size);
    if (newFd == -1)
        throw std::runtime_error("accept error");
    fcntl(newFd, F_SETFL, O_NONBLOCK); // non-block 설정
    EV_SET(&event, newFd, EVFILT_READ, EV_ADD, NULL, 0, NULL);
    kevent(this->kque, &event, 1, NULL, 0, NULL);

    Client client(newFd);
    socketFdToClient.insert(std::make_pair(newFd, client));
}

/* 서버가 클라이언트한테 소켓을 보낼 때*/
void Server::handleExistingConnection_send_client(int fd, struct kevent event)
{
    // 서버가 클라이언트한테 소캣을 보내는 경우,,?
    int send_data = send(fd, "awd", 3, 0);

    std::cout << "HERE!!\n";
    if (send_data < 0)
    {
        std::cout << "Error: Sending data failed" << std::endl;
        return;
    }
}

#define RED "\e[0;31m"
#define NC "\e[0m"

void Server::handleExistingConnection(int fd, struct kevent event)
{
    if (isConnected(fd, event) == false)
    {
        this->terminateConnection(fd, event);
        return;
    }

    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));

    int received = recv(fd, buffer, sizeof(buffer), 0);
    // 만약 데이터 수신이 실패한 경우 오류 메세지
    if (received < 0)
    {
        std::cout << "Error: Receving data failed" << std::endl;
        return;
    }
    std::cout << RED << buffer << NC << std::endl;

    std::vector<std::string> messages = split(buffer, '\n');

    for (int i = 0; i < messages.size(); i++)
    {
        execCommand(Message(fd, messages[i]));
    }
}

bool Server::isConnected(int fd, struct kevent event)
{
    if (event.flags & EV_EOF)
    {
        return false;
    }
    return true;
}

void Server::terminateConnection(int fd, struct kevent event)
{
    struct kevent temp_event;
    EV_SET(&temp_event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kque, &temp_event, 1, NULL, 0, NULL);
    close(fd);
    std::cout << fd << " : close!" << std::endl;
}

void Server::execCommand(Message message)
{
    if (message.getCommand() == "PASS")
        pass(message);
    else if (message.getCommand() == "NICK")
        nick(message);
    else if (message.getCommand() == "USER")
        user(message);
    else if (message.getCommand() == "PRIVMSG")
        privmsg(message);
    else if (message.getCommand() == "PING")
        ping(message);
    else if (message.getCommand() == "JOIN")
        join(message);
    // else if (message.getCommand() == "PART")
    //     part(message);
    // TODO : KICK, INVITE, MODE, TOPIC - operators only
    // TODO : PART, QUIT, EXIT
}

void Server::join(Message &message)
{
    // 채널 이름이 들어가야함. name
    if (message.getArg()[0].empty())
    {
        std::cout << "fuck error\n";
        return;
    }
    std::cout << "Here\n";
    
    std::string channel_name = message.getArg()[0];
    std::map<std::string, Channel>::iterator iter = this->channel.find(channel_name);

    // 채널이 없을 경우 생성
    if (iter == this->channel.end())
    {
        Channel newChannel(channel_name);
        newChannel.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 1);
        //this->channel[channel_name] = newChannel;
        channel.insert(make_pair(channel_name, newChannel));

        //출력해보기
        // iter = channel.begin();
        // for ( ; iter != channel.end(); iter++)
        // {
        //     std::cout << "channel -> channel_name : " << iter->first<<std::endl;
        //     iter->second.printMember();
        // }
        
    }
    // 채널이 있을 경우 그냥 들어가기
    else
    {
        std::cout << "원래 있다 쉐캬\n";
        iter->second.setMembers(socketFdToClient[message.getSocket()].getNickname(),
                         0);
    }
}

void Server::part(Message &message)
{
    if (message.getArg()[0].empty())
    {
        std::cout << "no arg error\n";
        return;
    }

    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = this->channel.find(channelName);
    if (iterCh == this->channel.end())    // 나갈 채널이 없을 경우
    {
        std::cout << channelName << ":No such channel\n";
        return ;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())     // 채널은 있는데 그 채널 속 유저가 아님
    {
        std::cout << channelName << ":You're not on that channel\n";
        return ;
    }
    else    //채널 있고, 그 채널 속 유저임 -> 채널 나갈거임 !
    {

    }
}

void Server::ping(Message &message)
{

    if (message.getArg()[0].empty())
    {
        const std::string &error_message = ":irc.local  461 * " +
                                           message.getCommand() +
                                           " :Not enough parameters";
        return;
    }
    std::cout << ":irc.local PONG " + message.getArg()[0] << std::endl;
    // pong 메세지 넣어주어야함.
    pong(message);
}

void Server::pong(Message &message)
{

    std::vector<std::string> textToBeSent;
    for (int i = 0; i < message.getArg().size(); i++)
        textToBeSent.push_back(message.getArg()[i]);
    std::string prefix =
        ":" + socketFdToClient[message.getSocket()].getNickname();

    Message toSendMessage(message.getSocket(), prefix, "PONG", textToBeSent);
    toSendMessage.sendToPong();
}

void Server::pass(Message &message)
{
    if (this->password != message.getArg()[0])
    {
        // std::cout << message.getArg()[0] << "password error" << std::endl;
        //  TODO : 비밀번호 에러 전송 후 종료
    }
}

void Server::nick(Message &message)
{
    int socket = message.getSocket();
    std::string newNickname = message.getArg()[0];

    std::map<int, Client>::iterator iter = socketFdToClient.find(socket);
    std::map<std::string, int>::iterator iterNicknameToSocket =
        nicknameToSocketFd.find(newNickname);

    if (iterNicknameToSocket == nicknameToSocketFd.end()) // nickname 중복 아님
    {
        if (iter->second.getNickname() !=
            "") // 이미 닉네임이 설정된 유저의 경우 - 변경
        {
            nicknameToSocketFd.erase(iter->second.getNickname());
        }
        iter->second.setNickname(newNickname);
        nicknameToSocketFd[newNickname] = message.getSocket();
    }
    else
    {
        std::cout << newNickname << " : nick 중복" << std::endl;
        // TODO : Nickname 중복 에러 전송
    }
}

void Server::user(Message &message)
{
    this->socketFdToClient[message.getSocket()].setUsername(
        message.getArg()[0]);
}

void Server::privmsg(Message &message)
{
    std::vector<std::string> receivers =
        split(message.getArg()[0], ','); // 수신자 여러명 쪼갬
    std::vector<std::string> textToBeSent;
    for (int i = 1; i < message.getArg().size();
         i++) // arg에서 수신자 닉네임 뒤 부터 다 집어넣음
        textToBeSent.push_back(message.getArg()[i]);
    std::string prefix =
        ":" + socketFdToClient[message.getSocket()].getNickname(); // 출처 추가

    for (int i = 0; i < receivers.size(); i++)
    {
        Message toSendMessage(nicknameToSocketFd[receivers[i]], prefix,
                              "PRIVMSG", textToBeSent);
        toSendMessage.sendToClient();
    }
}
