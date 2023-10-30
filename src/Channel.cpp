#include "../inc/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &name) : name(name)
{
    this->topicTitle = "";
}

const std::string &Channel::getName() const 
{
    return name;
}

const std::string &Channel::getTopicTitle() const 
{
    return topicTitle;
}

const  std::map<std::string, int>  &Channel::getMembers() const 
{
    return members;
}

void Channel::topic(const std::string &newtopicTitle) 
{
    this->topicTitle = newtopicTitle;
}

void Channel::mode()
{

}

int Channel::kick(const std::string &nickname)
{
    std::map<std::string, int>::iterator iter = members.find(nickname);
    
    if (iter == members.end()) // 유저 존재 x
    {
        std::cout << "no nickname" << std::endl;
        return 0;
    }
    else
    {
        members.erase(nickname);
        // TODO : kick 되었다고 전송?
    }
}

void Channel::invite(const std::string &nickname)
{
    members[nickname] = 0; // 기본 멤버로 초대
}