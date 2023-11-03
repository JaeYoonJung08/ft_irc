#include "../inc/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &name) : name(name)
{
    this->topic = "";
    this->key = "";
    this->limit = 0;
    MODE_I = false;
    MODE_T = false;
    // MODE_K = false;
    // MODE_L = false;
}

void Channel::setMembers(const std::string &nickname, const int operator_check)
{
    this->members[nickname] = operator_check;
    // members.insert(make_pair(nickname, operator_check));
}

const std::string &Channel::getName() const { return name; }

const std::string &Channel::getTopic() const { return topic; }

const std::string &Channel::getKey() const { return key; }

const int &Channel::getLimit() const { return limit; }

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

// const bool &Channel::getMODE_K() const
// {
//     return this->MODE_I;
// }

// const bool &Channel::getMODE_L() const { return this->MODE_I; }