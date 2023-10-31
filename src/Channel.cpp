#include "../inc/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &name) : name(name)
{
    this->topicTitle = "";
}

void Channel::setMembers(const std::string &nickname, const int operator_check)
{
    this->members[nickname] = operator_check;
    // members.insert(make_pair(nickname, operator_check));
}

const std::string &Channel::getName() const { return name; }

const std::string &Channel::getTopicTitle() const { return topicTitle; }

const std::map<std::string, int> &Channel::getMembers() const
{
    return members;
}

void Channel::topic(const std::string &newtopicTitle)
{
    this->topicTitle = newtopicTitle;
}

void Channel::mode() {}

void Channel::invite(const std::string &nickname)
{
    members[nickname] = 0; // 기본 멤버로 초대
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