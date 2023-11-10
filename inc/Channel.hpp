#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Message.hpp"
#include "Server.hpp"
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>

class Server;

class Channel
{
  private:
    static Server *serverPtr;

    std::string name;
    std::string topic;
    std::map<std::string, int> members; // 0 : member, 1 : operator
    std::string key;
    int memberLimit;
    bool MODE_I;
    bool MODE_T;
    std::set<std::string> invitedNickname;

  public:
    Channel();
    Channel(const std::string &name);
    ~Channel();

    static void setServerPtr(Server *server_ptr);

    void setMembers(const std::string &nickname, const int operator_check);
    void inviteNewMember(std::string nickname);
    bool isInvited(std::string nickname);
    void deleteMemberFromInvitedList(std::string nickname);
    bool isMember(const std::string& nickname);

    bool isNoOperator();

    const std::string &getName() const;
    std::string &getTopic();
    std::map<std::string, int> &getMembers();
    std::string &getKey();
    int &getLimit();

    bool &getMODE_I();
    bool &getMODE_T();

    void setMODE_I(bool mode);
    void setMODE_T(bool mode);

    void broadcasting(std::string fromNickname, Message &message);
    void partAll();
};

#endif
