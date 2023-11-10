#include "../inc/Command.hpp"

Command Command::commandInstance;

std::map<std::string, int> &Command::getServernicknameToSocketFd(void)
{
    return serverInstance->getNicknameToSocketFd();
}

std::map<int, Client> &Command::getServerSocketFdToClient(void)
{
    return serverInstance->getSocketFdToClient();
}

std::map<std::string, Channel> &Command::getServerChannel(void)
{
    return serverInstance->getChannel();
}

std::string &Command::getServerPassWord(void)
{
    return serverInstance->getPassWord();
}

const std::string &Command::getClientNickname(Message &message)
{
    return serverInstance->getSocketFdToClient()[message.getSocket()]
        .getNickname();
}

const std::string &Command::getClientUsername(Message &message)
{
    return serverInstance->getSocketFdToClient()[message.getSocket()]
        .getUsername();
}

void Command::password_incorrect_464(Message &message)
{
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    std::string nickname = client.getNickname();
    std::string error_message = ":irc_local 464 " + nickname +
                                " :Password Incorrect, Command disconnected";
    client.sendMessage(error_message);
}

void Command::empty_argument_431(Message &message)
{
    std::string error_message =
        ":irc_local 431 " + getClientNickname(message) + " :No nickname given";
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::command_empty_argument_461(Message &message)
{
    std::string error_message = ":irc_local 461 " + getClientNickname(message) +
                                " " + message.getCommand() +
                                ":Not enough parameters";
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::duplicate_check_433(Message &message)
{
    // std::string error_message = ":irc_local 433 " 
    //                              + message.getArg()[0] +
    //                             " :Nickname is already in use";
        std::string error_message = ":irc_local 433 " + message.getArg()[0] + " " +
                                message.getArg()[0] +
                                " :Nickname is already in use";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_such_channel_403(Message &message)
{
    std::string error_message = ":irc_local 403 " + getClientNickname(message) +
                                " " + message.getArg()[0] + " :No such channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::cannot_join_i_473(Message &message)
{
    std::string error_message = ":irc_local 473 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Cannot join channel (+i)";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::cannot_join_k_475(Message &message)
{
    std::string error_message = ":irc_local 475 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Cannot join channel (+k)";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::cannot_join_l_471(Message &message)
{
    std::string error_message = ":irc_local 473 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Cannot join channel (+l)";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::bad_channel_mask_476(Message &message)
{
    std::string error_message = ":irc_local 476 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Bad Channel Mask";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_member_channel_442(Message &message)
{
    std::string error_message = ":irc_local 442 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :You're not on that channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_operator_channel_482(Message &message)
{
    std::string error_message = ":irc_local 482 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :You're not channel operator";
    // serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(error_message);
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::no_users_channel_441(Message &message)
{
    // std::string error_message = ":irc_local 441 " + getClientNickname(message) +
    //                             " " + message.getArg()[0] + ' ' +
    //                             message.getArg()[1] +
    //                             " :They aren't on that channel";

            std::string error_message = ":irc_local 441 "+  getClientNickname(message) + " " + message.getArg()[1]  +
        " " + message.getArg()[0] +
        " :They aren't on that channel";

    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::user_already_channel_443(Message &message)
{
    std::string error_message = ":irc_local 443 " + getClientNickname(message) +
                                " " + message.getArg()[0] + ' ' +
                                message.getArg()[1] + " :is already on channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}



void Command::channel_mode_324(Message &message,
                                      Channel channel)
{
    std::string error_message =
        " :irc_local 324 " + getClientNickname(message) + " " + channel.getName();

    if (channel.getMODE_I())
        error_message + " +i";
    if (channel.getMODE_T())
        error_message + " +t";
    if (channel.getKey() != "")
        error_message + " +k";
    if (channel.getLimit() != 0)
    {
        // c 11인지 확인하기
        std::stringstream ss;
        ss << channel.getLimit();
        std::string tmp = ss.str();
        error_message + " +l" + tmp;
    }
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::characters_not_allowed_432(Message &message)
{
    std::string error_message = ":irc_local 432 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Erroneous nickname";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_topic_channel_331(Message &message)
{
    std::string reply = ":irc_local 331 " + getClientNickname(message) +
                                " " + message.getArg()[0] + " :No topic is set";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        reply);
}

void Command::yes_topic_channel_332(Message &message, std::string topic)
{
    std::string reply = ":irc_local 332 " + getClientNickname(message) +
                                " " + message.getArg()[0] + " " + topic;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        reply);
}

void Command::success_invite_341(Message &message, std::string newMemberName)
{
    std::string success_message =
        ":irc_local 341 " + getClientNickname(message) + " " +
        message.getArg()[0] + " " + message.getArg()[1];
    // std::string success_message =
    //     ":irc_local 341 " + message.getArg()[1]; + " " +
    //     message.getArg()[0];
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(success_message);
    //serverInstance->getClientByNickname(newMemberName).sendMessage(success_message);
}

void Command::no_such_server_402(std::string channelName, Message &message)
{
    std::string error_message = ":irc_local 402 " + getClientNickname(message) +
                                " " + channelName + " :No such server";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_nick_member_401(std::string no_nick, Message &message)
{
    std::string error_message =
        ":irc_local 401 " + no_nick + " :No such nick/channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_reciver_411(Message &message)
{
    std::string error_message = ":irc_local 411 " + getClientNickname(message) +
                                " " + ":No recipient given " +
                                message.getCommand();
    //"<client> :No recipient given (<command>)"
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_exist_message_412(Message &message)
{
    std::string error_message = ":irc_local 412 " + getClientNickname(message) +
                                " " + ":No text to send";
    //"<client> :No text to send"
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_member_channel_404(std::string channel, Message &message)
{
    std::string error_message = ":irc_local 404 " + getClientNickname(message) +
                                " " + channel + " :Cannot send to channel";
    //"<client> <channel> :Cannot send to channel"
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::error_no_nickname(Message &message)
{
    std::string error_message = ":irc_local : you don't have a nickname";
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::success_show_nickname(std::string nickname, Message &message)
{
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    std::string success_message =
        " :irc_local : your nickname " + client.getNickname();
    client.sendMessage(success_message);
}


void Command::join_success(Message &message, std::string channelName)
{
    std::string  suc_message = ":" + getClientNickname(message) + "!" + getClientUsername(message) + "@127.0.0.1 JOIN :" + channelName;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(suc_message);
}

void Command::join_RPL_NAMREPLY_353(Message &message, std::string channelName)
{
    std::map<std::string , Channel> &channel = getServerChannel();
    Channel &mini_channel = channel[channelName];

    std::map<std::string, int> &member = mini_channel.getMembers();

    std::map<std::string, int>::iterator iterMb = member.begin();
    std::string user_list;

    while (iterMb != member.end())
    {
        if (iterMb->second == 1)
            user_list += "@" + iterMb->first;
        else
            user_list += " " +iterMb->first;
        iterMb++;
    }
    
    std::cout << "user_list " << user_list << std::endl;

    std::string error_message = ":irc.local 353 " + getClientNickname(message) +
                                   " = " + message.getArg()[0] + " : " + user_list;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(error_message);
}

void Command::join_RPL_ENDOFNAMES_366(Message &message, std::string channelName)
{
    std::string ret = ":irc.local 366 "+ getClientNickname(message) + " " + channelName + " :End of /NAMES list.";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(ret);
}

//----------------------------command------------------------------//

void Command::pass(Message &message)
{
    // 비밀번호가 틀렸을 경우 ERR_PASSWDMISMATCH (464), 461을 464로 대체
    std::string password = getServerPassWord();

    int count = message.getArg().size();
    if (count == 0 || message.getArg()[0].empty() ||
        password != message.getArg()[0])
    {
        password_incorrect_464(message);
        serverInstance->terminateConnection(message.getSocket());
        throw std::runtime_error("pass error");
    }
}

void Command::nick(Message &message)
{
    int socket = message.getSocket();
    int count = message.getArg().size();
    // nick command만 나왔을 때 -> 현재 자신의 닉네임 보여줘야함.
    // 1. NICK을 했지만 클라이언트에 nick이 없을 때.
    // 2. NICK을 했지만 클라이언트에 nick이 있을 때.
    // 1번 먼제
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    Client &client = socketFdToClient[socket];
    if (count == 0 && client.getNickname().empty())
        return error_no_nickname(message);
    else if (count == 0)
        return success_show_nickname(client.getNickname(), message);
    std::string newNickname = message.getArg()[0];

    // Nickname이 empty일 때 431
    if (newNickname.empty())
        return empty_argument_431(message);
    std::map<int, Client>::iterator iter = socketFdToClient.find(socket);
    std::map<std::string, int> &nicknameToSocketFd = getServernicknameToSocketFd();
    std::map<std::string, int>::iterator iterNicknameToSocket = nicknameToSocketFd.find(newNickname);

    if (iterNicknameToSocket == nicknameToSocketFd.end()) // nickname 중복아님
    {
        //* ERR_ERRONEUSNICKNAME (432) -> NICK에 들어가면 안 되는 문자 == NULL,
        // CR, LF, space
        int i = 0;
        std::string nick = message.getArg()[0];
        while (nick[i++])
            if (nick[i] == '\r' || nick[i] == '\n' || nick[i] == ' ')
                return characters_not_allowed_432(message);
        if (iter->second.getNickname() != "") // 이미 닉네임이 설정된 유저의 경우 - 변경
        {
            nicknameToSocketFd.erase(iter->second.getNickname());
            iter->second.setNickname(newNickname);
            nicknameToSocketFd[newNickname] = message.getSocket();
            //: a!@ NICK :bb
            Message messageToSend(client.getSocket(),
                                  ":irc.local 001 " + client.getNickname() +
                                      " :change your nickname");
            // Message messageToSend(client.getSocket(), ":" + before + "!@" + "
            // NICK" + ":" + client.getNickname());
            client.sendMessage(messageToSend);
        }
        else
        {
            std::cout << "first newNick name : " << newNickname << std::endl;
            iter->second.setNickname(newNickname);
            nicknameToSocketFd[newNickname] = message.getSocket();
            Message messageToSend(
                client.getSocket(),
                ":irc.local 001 " + client.getNickname() +
                    " :Welcome to the Internet Relay Network");
            client.sendMessage(messageToSend);
        }
    }
    else
        return duplicate_check_433(
            message); // ERR_xNICKNAMEINUSE (433) -> Nickname 중복 에러 전송
}

void Command::user(Message &message)
{
    //    * ERR_NEEDMOREPARAMS (461)
    // 매개변수 충분하지 않음
    if (message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();

    Client &client = socketFdToClient[message.getSocket()];
    client.setUsername(message.getArg()[0]);
}

void Command::parrotmsg(Message &message)
{

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, int> &nicknameToSocketFd =
        getServernicknameToSocketFd();
    std::string fromNickname =
        socketFdToClient[message.getSocket()].getNickname();

    // std::vector<std::string> textToBeSent;
    //  자기 자신 nick 넣어주기
    // textToBeSent.push_back(fromNickname);
    std::string str;
    for (int i = 1; i < message.getArg().size(); i++)
    {
        std::string newstr;
        newstr += message.getArg()[i] + " ";
        std::cout << "str : " << str << std::endl;
        for (size_t j = 0; j < message.getArg()[i].size(); j++)
        {
            newstr[j] = std::toupper(
                static_cast<unsigned char>(newstr[j]));
        }
        str = str + newstr;
        // textToBeSent.push_back(str);
    }
    std::string sum = fromNickname + ": " + str;

    std::cout << "sum : " << sum << std::endl;
    std::string prefix = ":" + fromNickname; // 출처 추가

    Client &clientToSend = socketFdToClient[message.getSocket()];
    // Message messageToBeSent = Message(message.getSocket(), prefix, "PRIVMSG",
    // textToBeSent);

    clientToSend.sendMessage(sum);
}

void Command::privmsg(Message &message)
{
    // 이 부분은 여러 사용자들에게 메세지를 보낼 때 인 듯
    // 즉 첫 번째 자리에 여러 수신자들만 온 것 같음 이 부분이

    // ERR_NORECIPIENT (411) -> 수진자 지정 x
    if (message.getArg()[0].empty())
    {
        no_reciver_411(message);
        return;
    }

    std::vector<std::string> receivers =
        split(message.getArg()[0], ','); // 수신자 여러명 쪼갬

    // ERR_NOTEXTTOSEND (412) -> 보낼 텍스트가 없음.
    if (message.getArg()[1].empty())
    {
        no_exist_message_412(message);
        return;
    }
    std::vector<std::string> textToBeSent;
    for (int i = 0; i < message.getArg().size(); i++)
        textToBeSent.push_back(message.getArg()[i]);

    //- ERR_CANNOTSENDTOCHAN (404) -> 채널에 못 보낼 경우
    //- `PRIVMSG`/ 를 `NOTICE`에 전달할 수 없음 을 나타냅니다 `<channel>`. 이
    // 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다. 이는
    // 일반적으로 채널이 *[조정되고] 클라이언트가 채널에서 말할 수 있는 권한이
    // 없거나 *[외부 메시지 없음] * 모드가 설정된 채널에 참여하지 않는 등의 채널
    // 모드에 대한 응답으로 전송됩니다.

    // 1. 클라이언트가 채널에서 말할 수 없는 권한일 때 이 부분 추가해서 보자
    // MODE l 때 인 것 같은데 subject에 set이라고 있다.
    // 2. 클라이언트가 채널에 속해있지 않을 때

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, int> &nicknameToSocketFd =
        getServernicknameToSocketFd();
    std::string fromNickname =
        socketFdToClient[message.getSocket()].getNickname();
    std::string prefix = ":" + fromNickname; // 출처 추가

    // 통신 들어가기 전에 검사를 다 하고 가야됨.

    if (receivers[0][0] == '#' || receivers[0][0] == '&') // channel로 통신
    {

        std::map<std::string, Channel> &channel = getServerChannel();
        // ERR_NOSUCHSERVER (402) -> 채널이 없음
        for (unsigned int i = 0; i < receivers.size(); i++)
        {
            std::map<std::string, Channel>::iterator iter =
                channel.find(receivers[i].substr(0, receivers[i].size()));
            if (iter == channel.end())
            {
                no_such_server_402(receivers[i].substr(0, receivers[i].size()),
                                   message);
                return;
            }
            // 2. 클라이언트가 채널에 속해있지 않을 때, ERR_CANNOTSENDTOCHAN
            // (404)
            Client &nickname = socketFdToClient[message.getSocket()];
            std::map<std::string, int> member = iter->second.getMembers();
            if (member.find(nickname.getNickname()) == member.end())
            {
                no_member_channel_404(iter->first, message);
                return;
            }
        }
    }
    // 수신자들 통신
    else
    {
        for (unsigned int i = 0; i < receivers.size(); i++)
        {
            std::map<std::string, int>::iterator iter =
                nicknameToSocketFd.find(receivers[i]);
            if (iter == nicknameToSocketFd.end())
            {
                no_nick_member_401(receivers[i], message);
                return;
            }
        }
    }
    // 통신 시작
    for (int i = 0; i < receivers.size(); i++)
    {
        // 채널로 통신
        if (receivers[i][0] == '#' || receivers[i][0] == '&') // channel로 통신
        {
            serverInstance->getChannel()[receivers[i]].broadcasting(
                fromNickname, message);
        }
        // 수신자들 통신
        else
        {
            int socketToSend = nicknameToSocketFd[receivers[i]];
            Client &clientToSend = socketFdToClient[socketToSend];
            // std::cout << "nick:" <<  clientToSend.getNickname() << std::endl;
            Message messageToBeSent = Message(nicknameToSocketFd[receivers[i]],
                                              prefix, "PRIVMSG", textToBeSent);

            clientToSend.sendMessage(messageToBeSent);
        }
    }

    // 추가적인 부분해야될 거
    // 1. target 첫 번째 자리에 -> channel, 두 번째 자리에 -> 닉네임, 세 번째
    // 자리 -> text
}

void Command::ping(Message &message)
{
    // ERR_NEEDMOREPARAMS (461)
    if (message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    pong(message);
}

void Command::pong(Message &message)
{
    std::vector<std::string> textToBeSent;
    for (int i = 0; i < message.getArg().size(); i++)
        textToBeSent.push_back(message.getArg()[i]);

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::string prefix = ":irc.local";

    Message toSendMessage(message.getSocket(), prefix, "PONG", textToBeSent);

    socketFdToClient[message.getSocket()].sendMessage(toSendMessage);
}

int Command::joinChannelNameCheck(std::string name)
{
    if ((name[0] != '&' && name[0] != '#') || name.size() > 50)
        return false;
    int index = 0;
    while (name[index])
    {
        if (isspace(name[index] != 0) || name[index] == 7 || name[index] == ',')
            return false;
        index++;
    }
    return true;
}



void Command::join(Message &message)
{
    if (message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    std::map<std::string, Channel> &channel = getServerChannel();
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();

    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iter = channel.find(channelName);

    if (iter == channel.end())
    {
        if (joinChannelNameCheck(channelName) == false)
        {
            bad_channel_mask_476(message);
            return;
        }

        Channel newChannel(channelName);
        newChannel.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 1);
        // this->channel[channelName] = newChannel;
        channel.insert(make_pair(channelName, newChannel));

        //int socketToSend = nicknameToSocketFd[receivers[i]];
        Client &clientToSend = socketFdToClient[message.getSocket()];
        // std::cout << "nick:" <<  clientToSend.getNickname() << std::endl;
        // Message messageToBeSent = Message(":" + clientToSend.getNickname(),
        //                                     ":irc.local", "PRIVMSG", "aaa");


        join_success(message, channelName);
        yes_topic_channel_332(message, "");
        join_RPL_NAMREPLY_353(message, channelName);        

        //join 성공하고 난 후 366
        join_RPL_ENDOFNAMES_366(message, channelName);
        // 출력해보기
        //  iter = channel.begin();
        //  for ( ; iter != channel.end(); iter++)
        //  {
        //      std::cout << "channel -> channelName : " <<
        //      iter->first<<std::endl; iter->second.printMember();
        //  }
    }
    else
    {
        Client &clientToJoin = socketFdToClient[message.getSocket()];
        // 초대되지 않았는데 invited-only 에 시도할 경우
        if (iter->second.getMODE_I() && !iter->second.isInvited(clientToJoin.getNickname()))
        {
            cannot_join_i_473(message);
            return;
        }
        if (iter->second.getKey() != "")
        {
            if (message.getArg()[1] != iter->second.getKey())
            {
                cannot_join_k_475(message);
                return;
            }
        }
        if (iter->second.getLimit() != 0)
        {
            if (iter->second.getMembers().size() >= iter->second.getLimit())
            {
                cannot_join_l_471(message);
                return;
            }
        }
        iter->second.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 0);
        // 한번 들어갔으면 초대 리스트에서 제거
        iter->second.deleteMemberFromInvitedList(
            socketFdToClient[message.getSocket()].getNickname());


        

        //join 성공했을 때
        Message &reply = message;
        message.setPrefix(":" + clientToJoin.getNickname());
        iter->second.broadcasting(clientToJoin.getNickname(), reply);
        clientToJoin.sendMessage(reply);
        //join_success(message, channelName);
        yes_topic_channel_332(message, iter->second.getTopic());

        //join 성공하고 난 후 353 
        join_RPL_NAMREPLY_353(message,channelName);

        //join 성공하고 난 후 366
        join_RPL_ENDOFNAMES_366(message, channelName);


    }
}

//PART <channel> [<reason>]
void Command::part(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 채널은 있는데 그 채널 속 유저가 아님
    {
        no_member_channel_442(message);
        return;
    }
    else
    {
        members.erase(nickname);
        std::string cmd = "PART " + channelName;
        if (message.getArg()[1].length() > 1)
            cmd += " " + message.getArg()[1];
        Message reply(message.getSocket(), cmd);
        serverInstance->getChannel()[channelName].broadcasting(nickname, reply);
        std::map<std::string, int>::iterator iterOp = members.begin();
        while (iterOp != members.end())
        {
            if (iterOp->second == 1)
            // 전체 멤버 맵에서 다른 1 있는지 확인하기 -> 있으면 break
                break;
            iterOp++;
        }
        if (iterOp == members.end()) // 없으면 다른 사람들 모두 내보낸 뒤 채널 없애기
        {
            // 방장 나가서 채널없앤다고 경고메시지
            members.clear();
            std::map<std::string, Channel> &channels =
                this->serverInstance->getChannel();
            channels.erase(iterCh->first);
        }
    }
    return;
}

// +

// KICK <channel> <user> (<reason>) 인데 . . 여러명도 가능이라면 ? 자살
void Command::kick(Message &message)
{
    if (message.getArg().size() < 2 || message.getArg()[0].empty() ||
        message.getArg()[1].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    // 채널 있는지 확인
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();

    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        no_member_channel_442(message);
        return;
    }
    if (iterNick->second != 1) // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        no_operator_channel_482(message);
        return;
    }

    std::string kickName = message.getArg()[1];
    iterNick = members.find(kickName);
    if (iterNick == members.end()) // kick할 사람이 그 채널 속 유저가 아님
    {
        no_users_channel_441(message);
        return;
    }

    std::string cmd = "KICK " + channelName + " " + kickName;
    if (message.getArg()[2].length() > 1)
        cmd += " " + message.getArg()[2];
    Message reply(message.getSocket(), cmd);
    serverInstance->getChannel()[iterCh->first].broadcasting(nickname, reply);
    members.erase(kickName);
    std::map<std::string, int>::iterator iterOp = members.begin();
    while (iterOp != members.end())
    {
        if (iterOp->second == 1)
        // 전체 멤버 맵에서 다른 1 있는지 확인하기 -> 있으면 break
            break;
        iterOp++;
    }
    if (iterOp == members.end()) // 없으면 다른 사람들 모두 내보낸 뒤 채널 없애기
    {
        // 방장 나가서 채널없앤다고 경고메시지
        members.clear();
        std::map<std::string, Channel> &channels =
            this->serverInstance->getChannel();
        channels.erase(iterCh->first);
    }
    return;
}

// TOPIC <channel> (<topic>)
void Command::topic(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }
    
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }
    if (message.getArg().size() == 1)
    {
        if (channel[channelName].getTopic().empty())
            no_topic_channel_331(message);
        else
            yes_topic_channel_332(message, channel[channelName].getTopic());
        return;
    }

    if (channel[channelName].getMODE_T() && iterNick->second != 1)
    {
        no_operator_channel_482(message);
        return;
    }

    if (!message.getArg()[1].empty())
    {
        std::string &topic = channel[channelName].getTopic();
        topic = message.getArg()[1];
        yes_topic_channel_332(message, message.getArg()[1]);
        Message reply(message.getSocket(), "TOPIC " + channelName + " " + message.getArg()[1]);
        serverInstance->getChannel()[iterCh->first].broadcasting(nickname, reply);
    }
    return;
}

// INVITE <nickname> <channel>
void Command::invite(Message &message)
{
    if (message.getArg().size() < 2 || message.getArg()[0].empty() || message.getArg()[1].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    // 채널 있는지 확인
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();

    std::string channelName = message.getArg()[1];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    std::string &nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }
    if (iterNick->second != 1)
    {
        no_operator_channel_482(message);
        return;
    }

    std::string &newMemberName = message.getArg()[0];
    iterNick = members.find(newMemberName);
    if (iterNick != members.end()) // 초대할 사람이 이미 채널에 있음
    {
        user_already_channel_443(message);
        return;
    }
    // 초대할 사람이 서벙없는 경우 추가  . . 
    
    // 초대한 닉네임에 추가
    iterCh->second.inviteNewMember(newMemberName);

    //newMember 에게도 전송
    {
        int socketToSend = getServernicknameToSocketFd().find(newMemberName)->second;
        std::vector<std::string> argsToSend;
        argsToSend.push_back(newMemberName);
        argsToSend.push_back(channelName);
        Message messageToNewMember(socketToSend, ":" + nickname, "INVITE", argsToSend);
        socketFdToClient[socketToSend].sendMessage(messageToNewMember);
    }
    
    success_invite_341(message, newMemberName);
    return;
}

// MODE <channel> [+/-<mode> [<param>]]
void Command::mode(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }
    if (message.getArg()[0][0] != '&' && message.getArg()[0][0] != '#')
        return;

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();

    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }

    // 모드 설정 인자 없이 들어오면 현재 상태 알려주는 듯 !
    if (message.getArg().size() == 1)
    {
        channel_mode_324(message, iterCh->second);
        return;
    }

    // 모드 설정 인자 있고, 호출한 사람이 채널에 있긴 한데 방장이 아님
    if (iterNick->second != 1)
    {
        no_operator_channel_482(message);
        return;
    }

    if (message.getArg()[1][1] == 'o')
    {
        if (message.getArg().size() < 3 || message.getArg()[2].empty())
        {
            command_empty_argument_461(message);
            return;
        }
        std::string targetName = message.getArg()[2];
        iterNick = members.find(targetName);
        if (iterNick == members.end()) // 설정할 사람이 채널에 없음
        {
            no_nick_member_401(message.getArg()[2], message);
            return;
        }
        else if (message.getArg()[1][0] == '+')
            members[targetName] = 1;
        else if (message.getArg()[1][0] == '-')
            members[targetName] = 0;
        else
        {
            // wrong arg
            return;
        }
    }

    else if (message.getArg()[1][1] == 'k')
    {
        if (message.getArg().size() < 3 || message.getArg()[2].empty())
        {
            command_empty_argument_461(message);
            return;
        }
        std::string &oldPassword = iterCh->second.getKey();
        std::string newPassword = message.getArg()[2];
        if (message.getArg()[1][0] == '+' && oldPassword == "")
            oldPassword = newPassword;
        else if (message.getArg()[1][0] == '-' && oldPassword == newPassword)
            oldPassword = "";
        else
        {
            // +/- 아니거나, 이미 key 있는데 key 등록, -k인데 패스워드 틀림
            return;
        }
    }

    else if (message.getArg()[1][1] == 'l')
    {
        if (message.getArg()[1][0] == '+' && message.getArg()[2].empty())
        {
            command_empty_argument_461(message);
            return;
        }

        int &oldLimit = iterCh->second.getLimit();
        int newLimit = atoi(message.getArg()[2].c_str());
        if (message.getArg()[1][0] == '+')
        {
            // 현재 채널 멤버보다 limit이 적으면 안되게 설정
            if (newLimit < members.size())
            {
                // fail
                return;
            }
            oldLimit = newLimit;
        }
        else if (message.getArg()[1][0] == '-')
            oldLimit = 0;
        else
        {
            // +/- 아님 wrong argument
            return;
        }
    }

    else if (setMode(message, iterCh->second) == false)
    {
        // +/- 아니거나 i/t 아니면 wrong argument
        return;
    }
    std::string cmd = "MODE " + channelName + " " + message.getArg()[1];
    if (message.getArg().size() > 2)
        cmd += " :" + message.getArg()[2];
    Message reply(message.getSocket(), cmd);
    serverInstance->getChannel()[iterCh->first].broadcasting(nickname, reply);
    return;
}

bool Command::setMode(Message &message, Channel &channel)
{
    bool MODE;

    if (message.getArg()[1][0] == '+')
        MODE = true;
    else if (message.getArg()[1][0] == '-')
        MODE = false;
    else
        return false;
        
    if (message.getArg()[1][1] == 'i')
        channel.setMODE_I(MODE);
    else if (message.getArg()[1][1] == 't')
        channel.setMODE_T(MODE);
    else
        return false;

    return true;
}

void Command::quit(Message &message)
{
    serverInstance->terminateConnection(message.getSocket());
}