#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include "Server.hpp"
#include "Message.hpp"

class Server;

class Channel
{
  private:
    static Server *serverPtr;

    std::string name;
    std::string topic;
    std::map<std::string, int> members; // 0 : member, 1 : operator
    std::string key;
    int limit;
    bool MODE_I;
    bool MODE_T;
    // bool MODE_K;
    // bool MODE_L;

  public:
    Channel();
    Channel(const std::string &name);
    ~Channel();

    static void setServerPtr(Server *server_ptr);

    void setMembers(const std::string &nickname, const int operator_check);

    const std::string &getName() const;
    const std::string &getTopic() const;
    const std::map<std::string, int> &getMembers() const;
    const std::string &getKey() const;
    const int &getLimit() const;

    const bool &getMODE_I() const;
    const bool &getMODE_T() const;
    // const bool &Channel::getMODE_K() const;
    // const bool &Channel::getMODE_L() const;

    void printMember();
    void broadcasting(std::string &fromNickname, Message &message);
};

#endif
