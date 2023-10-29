#include "../inc/IrcServ.hpp"

IrcServ::IrcServ()
{

}

IrcServ::IrcServ(int portNumber, std::string password)
{
	this->portNumber = portNumber;
	this->password = password;

	this->kque = kqueue(); // kque 생성
	if (this->kque == -1)
		throw std::runtime_error("kqueue error");
}

void IrcServ::openSocket()
{
	sockaddr_in serverAddr;

	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0); //TCP socket 생성
	if (this->serverSocket == -1)
		throw std::runtime_error("socket error");

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 임시로 일단은 모든 주소 받음
	serverAddr.sin_port = htons(this->portNumber);

	//port 재사용시 bind error 없애는 옵션 추가.
	int option = 1;
	setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	//bind 를 통해 socket이랑 각 설정이랑 연결
	if (bind(this->serverSocket, reinterpret_cast<const sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
		throw std::runtime_error("bind error");
	listen(this->serverSocket, LISTEN_BACKLOG_NUM); // 수신하도록 listen
	// tcp 연결 완료.
}

void IrcServ::init()
{
	struct kevent event;

	EV_SET(&event, this->serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(this->kque, &event, 1, NULL, 0, NULL) == -1)
		throw std::runtime_error("kevent attach error");
}

IrcServ::~IrcServ()
{
	close(serverSocket);
	close(kque);
}

void IrcServ::run()
{
	while (true)
	{
		struct kevent events[32];
		int triggered = kevent(this->kque, NULL, 0, events, 32, NULL);
		if (triggered == -1)
			throw std::runtime_error("kevent event error");
		for (int i = 0; i < triggered; i++)
		{
			if ((int) events[i].ident == this->serverSocket) // 소켓에서 이벤트 발생
			{
				std::cout << "새로운 접속!" << std::endl;
				//새로운 접속 처리
				handleNewConnection(events[i].ident);
			} else
			{ // 기존 접속
				std::cout << "기존 접속!" << std::endl;
				handleExistingConnection(events[i].ident);
			}
		}
	}
}

void IrcServ::handleNewConnection(int sockFd)
{
	struct sockaddr client_addr;
	struct kevent event;
	int client_addr_size = sizeof(client_addr);

	int newFd = accept(sockFd, (sockaddr *) &client_addr, (socklen_t *) &client_addr_size);
	if (newFd == -1)
		throw std::runtime_error("accept error");
	EV_SET(&event, newFd, EVFILT_READ, EV_ADD, NULL, 0, NULL);
	kevent(this->kque, &event, 1, NULL, 0, NULL);

}

void IrcServ::handleExistingConnection(int fd)
{
	char data[1024];
	if (read(fd, data, 1024) == -1)
		throw std::runtime_error("read error");

	std::cout << data << std::endl;
}
