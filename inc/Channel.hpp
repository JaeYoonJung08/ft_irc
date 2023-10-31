#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <iterator>
#include <map>
#include <string>

class Channel
{
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
    void invite(const std::string &nickname);
    void printMember();
};

#endif
