#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <string>


class Channel {
private:
    std::string name;
    std::string topicTitle;
    std::list<std::string> members;
    std::list<std::string> operators;


public:
    Channel();
    Channel(const std::string &name);
    ~Channel();



    const std::string &getName() const;
    const std::string &getTopicTitle() const;
    const std::list<std::string> &getMembers() const;
    const std::list<std::string> &getOperators() const;

    void topic();
    void mode();
    void kick();
    void invite();
};


#endif
