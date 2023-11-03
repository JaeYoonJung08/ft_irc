#include "../inc/Client.hpp"

Client::Client()
{
}

Client::Client(int socket)
{
    this->socket = socket;
    nickname = "";
    username = "";
    inBuffer = "";
}

// Client::Client(int socket, const std::string &nickname,
//                const std::string &username)
//     : socket(socket), nickname(nickname), username(username)
// {
// }

void Client::setUsername(std::string username)
{
    this->username = username;
}

void Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

// Client::

Client::~Client() {}
const std::string &Client::getNickname() const
{
    return nickname;
}

#define RED "\e[0;31m"
#define NC "\e[0m"

std::vector<Message> Client::readData()
{
    std::vector<Message> ret;

    char buffer[IN_BUFFER_SIZE];

    //memset(buffer, 0, sizeof(buffer));
    bzero(buffer, IN_BUFFER_SIZE);

    int received = recv(this->socket, buffer, sizeof(buffer), 0);
    // 만약 데이터 수신이 실패한 경우 오류 메세지
    if (received < 0)
        throw std::runtime_error("recv() error");

    inBuffer.append(buffer);

    std::cout << RED << inBuffer << NC << std::endl;

    ret = this->extractMessageFromBuffer();

    return ret;
}
std::vector<Message> Client::extractMessageFromBuffer()
{
    std::vector<std::string> messages;

    std::string::size_type pos = inBuffer.find("\r\n");
    while (pos != std::string::npos)
    {
        messages.push_back(inBuffer.substr(0, pos));
        inBuffer.erase(0, pos + 2);
        pos = inBuffer.find("\r\n");
    }

    std::vector<Message> ret;

    for (int i = 0; i < messages.size(); i++)
        ret.push_back(Message(this->socket, messages[i]));

    return ret;
}
