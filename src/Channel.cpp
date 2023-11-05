#include "../inc/Channel.hpp"

Server *Channel::serverPtr;

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &name) : name(name)
{
    this->topic = "";
    this->key = "";
    this->memberLimit = 0;
    MODE_I = false;
    MODE_T = false;
}

void Channel::setMembers(const std::string &nickname, const int operator_check)
{
    this->members[nickname] = operator_check;
    // members.insert(make_pair(nickname, operator_check));
}

const std::string &Channel::getName() const { return name; }

const std::string &Channel::getTopic() const { return topic; }

const std::string &Channel::getKey() const { return key; }

const int &Channel::getLimit() const { return memberLimit; }

const std::map<std::string, int> &Channel::getMembers() const
{
    return members;
}

void Channel::printMember()
{
    std::map<std::string, int>::iterator iter = this->members.begin();

    std::cout << iter->first << std::endl;
    // for (; iter != members.end(); iter++)
    // {
    //     std::cout << "channel -> member : " << iter->first << std::endl;
    // }
}

const bool &Channel::getMODE_I() const { return this->MODE_I; }

const bool &Channel::getMODE_T() const { return this->MODE_T; }

void Channel::broadcasting(std::string &fromNickname, Message &message)
{
    std::map<std::string, int>::iterator iter = this->members.begin();
    while (iter != members.end())
    {
        Client &clientToSend = this->serverPtr->getClientByNickname(iter->first);
        if (fromNickname != iter->first)
        {
            Message messageToSend = message;
            messageToSend.setPrefix(":" + fromNickname);
            clientToSend.sendMessage(messageToSend);
        }
        iter++;
    }
}

void Channel::setServerPtr(Server *server_ptr)
{
    serverPtr = server_ptr;
}