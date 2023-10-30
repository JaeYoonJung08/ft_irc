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
                //naki님이 말씀하신 서버에서 클라이언트로 우째 보내노 라는 질문을
                //여기서 해결할 것임
                //kevent 구조체 안에는 filter라는 게 있음 이 친구들을 통해 
                //서버에서 소켓을 받는지, 클라이언트한테 보내줄 것인지를 결정
                if (events[i].filter == EVFILT_READ)
                {
                    //기존 handleExistingConnection
                    //여기 이 부분이 클라이언트가 서버로 패킷을 보낼 경우

                    //여기서 이제해야할 거 개행을 찾지 못한 경우와 개행을 찾은 경우을 나누어야함.
                    //함수 수정해주자
                    handleExistingConnection(events[i].ident, events[i]);
                }
                else if (events[i].filter == EVFILT_WRITE)
                {
                    //새롭게 추가 된 거
                    //서버가 클라리언트로 패킷을 보낼 경우
                    handleExistingConnection_send_client(events[i].ident, events[i]);
                }
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
    EV_SET(&event, newFd, EVFILT_READ, EV_ADD, NULL, 0, NULL);
    kevent(this->kque, &event, 1, NULL, 0, NULL);

    Client client(newFd);
    socketFdToClient.insert(std::make_pair(newFd, client));
}


/* 서버가 클라이언트한테 소켓을 보낼 때*/
void Server::handleExistingConnection_send_client(int fd, struct kevent event)
{
    //서버가 클라이언트한테 소캣을 보내는 경우,,?
    int send_data = send(fd, "awd", 3,0);


    std::cout << "HERE!!\n";
    if (send_data < 0)
    {
        std::cout << "Error: Sending data failed" << std::endl;
        return ;
    }
}

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

    execCommand(Message(fd, buffer));

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
    {
        pass(message);
    }
    else if (message.getCommand() == "NICK")
    {
        nick(message);
    }
    else if (message.getCommand() == "USER")
    {
        user(message);
    }
    else if (message.getCommand() == "PRIVMSG")
    {
        privmsg(message);
    }
    // 등등...
}

void Server::pass(Message &message)
{
    if (this->password != message.getArg()[0])
    {
        //std::cout << message.getArg()[0] << "password error" << std::endl;
        // TODO : 비밀번호 에러 전송 후 종료
    }
}

void Server::nick(Message &message)
{
    this->socketFdToClient[message.getSocket()]
        .setNickname(message.getArg()[0]);
}

void Server::user(Message &message)
{
    this->socketFdToClient[message.getSocket()]
        .setUsername(message.getArg()[0]);
}

void Server::privmsg(Message &message)
{
    write(message.getSocket(), message.getArg()[0].c_str(), message.getArg()[0].size());
    // TODO : 닉네임 받고 그 닉네임에 맞는 fd 찾고 그거에 다가 메시지 전송
}

