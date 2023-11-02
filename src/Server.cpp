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
    else if (message.getCommand() == "PART")
        part(message);
    else if (message.getCommand() == "KICK")
        kick(message);
    else if (message.getCommand() == "TOPIC")
        topic(message);
    else if (message.getCommand() == "INVITE")
        topic(message);
    // TODO : MODE - operators only
    // TODO : QUIT, EXIT
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
    std::map<std::string, Channel>::iterator iter =
        this->channel.find(channel_name);

    // 채널이 없을 경우 생성
    if (iter == this->channel.end())
    {
        Channel newChannel(channel_name);
        newChannel.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 1);
        // this->channel[channel_name] = newChannel;
        channel.insert(make_pair(channel_name, newChannel));

        // 출력해보기
        //  iter = channel.begin();
        //  for ( ; iter != channel.end(); iter++)
        //  {
        //      std::cout << "channel -> channel_name : " <<
        //      iter->first<<std::endl; iter->second.printMember();
        //  }
    }
    // 채널이 있을 경우 그냥 들어가기
    else
    {
        std::cout << "원래 있다 쉐캬\n";
        // invite만 가능한 방
        if (iter->second.getMODE_I())
        {
            // 에러 처리 해주기
            return;
        }
        // 채널이 비밀번호가 있을 때
        if (iter->second.getKey() != "")
        {
            if (message.getArg()[1] == iter->second.getKey())
            {
                iter->second.setMembers(
                    socketFdToClient[message.getSocket()].getNickname(), 0);
            }
            else // 비밀 번호 틀렷을 때 에러 처리
            {

                return;
            }
        }
        iter->second.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 0);
    }
}

void Server::part(Message &message) // cout 한 것 전부 send(error)로 바꿔야 !
{
    if (message.getArg()[0].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
        return;
    }

    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh =
        this->channel.find(channelName);
    if (iterCh == this->channel.end()) // 나갈 채널이 없을 경우
    {
        // error 403 "<client> <channel> :No such channel"
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 채널은 있는데 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        return;
    }
    else // 채널 있고, 그 채널 속 유저임 -> 채널 나갈거임 ! 만약 방장이라면 ? ..
    {
        if (iterNick->second == 1)
            ; // 방장이라면 : 다른 사람에게 방장 물려주기 / 다른 방장 x시 방
              // 폭파
        members.erase(nickname); // members에서 iterNick 빼기 . .
    }
    return;
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

// KICK <channel> <user> (<reason>) 인데 . . 여러명도 가능이라면 ? 자살
void Server::kick(Message &message)
{
    // 인자 있는지 확인
    std::string a = NULL;

    if (message.getArg()[0].empty() || message.getArg()[1].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
        return;
    }

    // 채널 있는지 확인
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh =
        this->channel.find(channelName);
    if (iterCh == this->channel.end())
    {
        // error 403 "<client> <channel> :No such channel"
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        return;
    }
    if (iterNick->second != 1) // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        // error 482 "<client> <channel> :You're not channel operator"
        return;
    }

    std::string kickName = message.getArg()[1];
    iterNick = members.find(kickName);
    if (iterNick == members.end()) // kick할 사람이 그 채널 속 유저가 아님
    {
        // error 441 "<client> <nick> <channel> :They aren't on that channel"
        return;
    }

    members.erase(kickName);
    // 관련 메시지 전송
    return;
    /*
        std::map<std::string, int>::iterator iter = members.find(nickname);

        if (iter == members.end()) // 유저 존재 x
        {
            std::cout << "no nickname" << std::endl;
            return;
        }
        else
        {
            members.erase(nickname);
            // TODO : kick 되었다고 전송?
        }
        return;
    */
}

// TOPIC <channel> (<topic>)
void Server::topic(Message &message)
{
    // 인자 있는지 확인
    if (message.getArg()[0].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
        return;
    }

    // 채널 있는지 확인
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh =
        this->channel.find(channelName);
    if (iterCh == this->channel.end())
    {
        // error 403 "<client> <channel> :No such channel"
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        return;
    }
    if (iterCh->second.getMODE_T() && iterNick->second != 1)
    // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        // error 482 "<client> <channel> :You're not channel operator"
        return;
    }

    // 주제 인자 있는지 확인 -> 없으면 RPL_NOTOPIC (331)
    std::string topic = iterCh->second.getTopic();
    topic = message.getArg()[1]; // 이거 없으면 세그나니까 에러처리 필수
    // 관련 메시지 전송
    return;
}

// INVITE <nickname> <channel>
void Server::invite(Message &message)
{
    // 인자 모두 있는지 확인
    if (message.getArg()[0].empty() || message.getArg()[1].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
        return;
    }

    // 채널 있는지 확인
    std::string channelName = message.getArg()[1];
    std::map<std::string, Channel>::iterator iterCh =
        this->channel.find(channelName);
    if (iterCh == this->channel.end())
    {
        // error 403 "<client> <channel> :No such channel"
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        return;
    }
    if (iterNick->second != 1) // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        // error 482 "<client> <channel> :You're not channel operator"
        return;
    }

    std::string newMemberName = message.getArg()[0];
    iterNick = members.find(newMemberName);
    if (iterNick != members.end()) // 초대할 사람이 이미 채널에 있음
    {
        // error 443 : "<client> <nick> <channel> :is already on channel"
        return;
    }
    iterCh->second.setMembers(newMemberName, 0);
    // 관련 메시지 전송
    return;
    /*
        members[nickname] = 0; // 기본 멤버로 초대
    */
}

// MODE <channel> +/-<mode> (<param>)
void Server::mode(Message &message)
{
    // 인자 있는지 확인
    if (message.getArg()[0].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
        return;
    }

    // 채널 있는지 확인
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh =
        this->channel.find(channelName);
    if (iterCh == this->channel.end())
    {
        // error 403 "<client> <channel> :No such channel"
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        return;
    }

    // 모드 설정 인자 없이 들어오면 현재 상태 알려주는 듯 !
    if (message.getArg()[1].empty())
    {
        // message 324  "<client> <channel> <modestring> <mode arguments>..."
        return;
    }

    // 모드 설정 인자 있고, 호출한 사람이 채널에 있긴 한데 방장이 아님
    if (iterNick->second != 1)
    {
        // error 482 "<client> <channel> :You're not channel operator"
        return;
    }

    if (message.getArg()[1][1] == 'o')
    {
        if (message.getArg()[2].empty())
        {
            // no param
            return;
        }
        std::string targetName = message.getArg()[2];
        iterNick = members.find(targetName);
        if (iterNick == members.end()) // 설정할 사람이 채널에 없음
        {
            // error
            return;
        }
        else if (message.getArg()[1][0] == '+')
            members[targetName] = 1;
        else if (message.getArg()[1][0] == '-')
            members[targetName] = 0;
        else
        {
            // wrong arg
            return;
        }
    }

    else if (message.getArg()[1][1] == 'k')
    {
        if (message.getArg()[2].empty())
        {
            // no param
            return;
        }
        std::string oldPassword = iterCh->second.getKey();
        std::string newPassword = message.getArg()[2];
        if (message.getArg()[1][0] == '+')
            oldPassword = newPassword;
        else if (message.getArg()[1][0] == '-' && oldPassword == newPassword)
            oldPassword = "";
        else
        {
            // +/- 아니거나, -k인데 패스워드 틀림
            return;
        }
    }

    else if (message.getArg()[1][1] == 'l')
    {
        if (message.getArg()[1][0] == '+' && message.getArg()[2].empty())
        {
            // no param
            return;
        }

        int oldLimit = iterCh->second.getLimit();
        int newLimit = atoi(message.getArg()[2].c_str());
        if (message.getArg()[1][0] == '+')
        {
            // 현재 채널 멤버보다 limit이 적으면 안되게 설정
            if (newLimit < members.size())
            {
                // fail
                return;
            }
            oldLimit = newLimit;
        }
        else if (message.getArg()[1][0] == '-')
            oldLimit = 0;
        else
        {
            // +/- 아님
            return;
        }
    }

    else if (setMode(message, iterCh->second) == false)
    {
        // +/- 아니거나 i/t 아니면 wrong argument
        return;
    }
    // 관련 메시지 전송
    return;
}

bool Server::setMode(Message &message, Channel channel)
{
    bool MODE;

    if (message.getArg()[1][1] == 'i')
        MODE = channel.getMODE_I();
    else if (message.getArg()[1][1] == 't')
        MODE = channel.getMODE_T();
    else
        return false;

    if (message.getArg()[1][0] == '+')
        MODE = true;
    else if (message.getArg()[1][1] == '-')
        MODE = false;
    else
        return false;

    return true;
}