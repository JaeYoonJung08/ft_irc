#include "../inc/Channel.hpp"


Channel::Channel() {}

Channel::~Channel() {}


Channel::Channel(const std::string &name) : name(name)
{
    this->topicTitle = "";
}

const std::string &Channel::getName() const {
    return name;
}

const std::string &Channel::getTopicTitle() const {
    return topicTitle;
}

const std::list<std::string> &Channel::getMembers() const {
    return members;
}

const std::list<std::string> &Channel::getOperators() const {
    return operators;
}
