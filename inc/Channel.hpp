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
    std::string topic;
    std::map<std::string, int> members; // 0 : member, 1 : operator
    std::string password;
    bool  MODE_I;
    bool  MODE_T;
    bool  MODE_K;
    bool  MODE_O;
    bool  MODE_L;

  public:
    Channel();
    Channel(const std::string &name);
    ~Channel();

    void setMembers(const std::string &nickname, const int operator_check);

    const std::string &getName() const;
    const std::string &getTopic() const;
    const std::map<std::string, int> &getMembers() const;
    const std::string &Channel::getPassword() const;

    const bool &Channel::getMODE_I() const;
    const bool &Channel::getMODE_T() const;
    const bool &Channel::getMODE_K() const;
    const bool &Channel::getMODE_O() const;
    const bool &Channel::getMODE_L() const;

    void printMember();
};

#endif
