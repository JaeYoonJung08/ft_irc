#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <string>
#include <iterator>


class Channel {
private:
    std::string name;
    std::string topicTitle;
    std::map<std::string, int> members; // 0 : member, 1 : operator


public:
    Channel();
    Channel(const std::string &name);
    ~Channel();


    void setMembers(const std::string &nickname, const int operator_check);

    const std::string &getName() const;
    const std::string &getTopicTitle() const;
    const std::map<std::string, int> &getMembers() const;


    void topic(const std::string &newtopicTitle);
    void mode();
    int kick(const std::string &nickname);
    void invite(const std::string &nickname);
};


#endif
