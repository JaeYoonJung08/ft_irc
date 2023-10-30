#include "../inc/Client.hpp"

Client::Client(){}

Client::Client(int socket)
{
    this->socket = socket;
    nickname = "";
    username = "";
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
