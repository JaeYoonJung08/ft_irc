#include "../inc/Command.hpp"

Command Command::commandInstance;

void Command::password_incorrect_464(Message &message)
{
    // std::string nick_msg = ":irc_local 464 User :Password Incorrect, Command
    // disconnected";
    //"<clint> :Password incorrect"

    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    std::string nickname = client.getNickname();
    std::string error_message = ":irc_local 464 " + nickname +
                                " :Password Incorrect, Command disconnected";

    client.sendMessage(error_message);
}

void Command::command_empty_argument_461(Message &message)
{
    std::string error_message = " :irc_local 461 :Not enough parameters";
    //"<client> <command> :Not enough parameters"
    std::cout << message.getSocket() << " " << message.getCommand()
              << error_message << std::endl;
}

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

void Command::duplicate_check_433(Message &message)
{
    std::string error_message = ":irc_local 433 " + message.getArg()[0] + " " +
                                message.getArg()[0] +
                                " :Nickname is already in use";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::empty_argument_431(Message &message)
{
    std::string error_message = " :irc_local 431 :No nickname given";
    //"<client> :No nickname given"
    std::cout << message.getSocket() << error_message << std::endl;
}

void Command::no_such_channel_403(Message &message)
{
    std::string error_message = " :irc_local 403 :No such channel";
    //  "<client> <channel> :No such channel"
    std::cout << message.getSocket() << " " << message.getArg()[0]
              << error_message << std::endl;
}

void Command::no_member_channel_442(Message &message)
{
    std::string error_message = " :irc_local 442 :You're not on that channel";
    //"<client> <channel> :You're not on that channel"
    std::cout << message.getSocket() << " " << message.getArg()[0]
              << error_message << std::endl;
}

void Command::no_operator_channel_482(Message &message)
{

    std::string error_message = " :irc_local 482 :You're not channel operator";
    // error 482 "<client> <channel> :You're not channel operator"
    std::cout << message.getSocket() << " " << message.getArg()[0]
              << error_message << std::endl;
}

void Command::no_users_channel_441(Message &message)
{
    std::string error_message = " :irc_local 441 :They aren't on that channel";
    // error 441 "<client> <nick> <channel> :They aren't on that channel"
    std::cout << message.getSocket() << " " << message.getArg()[0] << " "
              << message.getArg()[1] << error_message << std::endl;
}

void Command::user_already_channel_443(Message &message)
{
    std::string error_message = " :irc_local 443 :is already on channel";
    // error 443 "<client> <nick> <channel> :is already on channel"
    std::cout << message.getSocket() << " " << message.getArg()[0] << " "
              << message.getArg()[1] << error_message << std::endl;
}

void Command::state_without_setup_324(Message &message)
{
    std::string error_message = " :irc_local 324 ";
    // error 324 "<client> <channel> <modestring> <mode arguments>..."
    std::cout << message.getSocket() << " " << message.getArg()[0] << " "
              << message.getArg()[1] << " " << message.getArg()[2] << " ";

    int i = 3;
    while (i < message.getArg().size())
    {
        std::cout << message.getArg()[i] << " ";
        i++;
    }
    std::cout << error_message << std::endl;
}

void Command::characters_not_allowed_432(Message &message)
{
    std::string error_message = " :irc_local 432 :Erroneus nickname";
    // error 432 "<client> <nick> :Erroneus nickname"
    std::cout << message.getArg()[0] << error_message << std::endl;
}

void Command::no_topic_channel_331(Message &message)
{
    std::string error_message = " :irc_local 432 :No topic is set";
    // error 331 "<client> <channel> :No topic is set"
    std::cout << message.getArg()[0] << error_message << std::endl;
}

void Command::success_invite_341(Message &message)
{
    std::string success_message = " :irc_local SUCCESS ";
    // error 331 "<client> <channel> :No topic is set"
    std::cout << success_message << message.getArg()[1] << " "
              << message.getArg()[0] << std::endl;
}

void Command::no_such_server_402(std::string channel_name)
{
    std::string error_message = " :irc_local :No such server";
    //"<client> <server name> :No such server"
    std::cout << channel_name << " " << error_message << std::endl;
}

void Command::no_nick_member_401(std::string no_nick)
{
    std::string error_message = " :irc_local :No such nick/channel";
    //"<client> <nickname> :No such nick/channel"
    std::cout << no_nick << error_message << std::endl;
}

void Command::no_reciver_411(Message &message)
{
    std::string error_message = " :irc_local :No recipient given ";
    //"<client> :No recipient given (<command>)"
    std::cout << error_message << message.getCommand() << std::endl;
}

void Command::no_exist_message_412(Message &message)
{
    std::string error_message = " :irc_local :No text to send";
    //"<client> :No text to send"
    std::cout << error_message << std::endl;
}

void Command::no_member_channel_404(std::string channel)
{
    std::string error_message = " :irc_local :Cannot send to channel";
    //"<client> <channel> :Cannot send to channel"
    std::cout << channel << error_message << std::endl;
}

//----------------------------command------------------------------//

void Command::pass(Message &message)
{
    // 비밀번호가 틀렸을 경우 ERR_PASSWDMISMATCH (464), 461을 464로 대체
    std::string password = getServerPassWord();
    if (password != message.getArg()[0] || message.getArg()[0].empty())
    {
        password_incorrect_464(message);
        serverInstance->terminateConnection(message.getSocket());
        throw std::runtime_error("pass error");
    }
}

void Command::nick(Message &message)
{
    int socket = message.getSocket();
    std::string newNickname = message.getArg()[0];

    // Nickname이 empty일 때 431
    if (newNickname.empty())
    {
        empty_argument_431(message);
        return;
    }

    // 추가
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<int, Client>::iterator iter = socketFdToClient.find(socket);

    // 추가
    std::map<std::string, int> &nicknameToSocketFd =
        getServernicknameToSocketFd();
    std::map<std::string, int>::iterator iterNicknameToSocket =
        nicknameToSocketFd.find(newNickname);

    if (iterNicknameToSocket == nicknameToSocketFd.end()) // nickname 중복아님
    {
        if (iter->second.getNickname() !=
            "") // 이미 닉네임이 설정된 유저의 경우 - 변경
        {
            nicknameToSocketFd.erase(iter->second.getNickname());
        }
        std::cout << "nickname : " << newNickname << std::endl;
        iter->second.setNickname(newNickname);
        nicknameToSocketFd[newNickname] = message.getSocket();
    }
    else
    {
        std::cout << newNickname << " : nick 중복" << std::endl;
        // TODO : Nickname 중복 에러 전송
        // ERR_NICKNAMEINUSE (433)
        duplicate_check_433(message);
        return;
    }

    //* ERR_ERRONEUSNICKNAME (432)
    // NICK에 들어가면 안 되는 문자 == NULL, CR, LF, space
    int i = 0;
    std::string nick = message.getArg()[0];
    while (nick[i])
    {
        if (nick[i] == '\r' || nick[i] == '\n' || nick[i] == ' ')
            characters_not_allowed_432(message);
        i++;
    }

    Client &client = socketFdToClient[message.getSocket()];

    Message messageToSend(client.getSocket(),
                          ":irc.local 001 " + client.getNickname() +
                              " :Welcome to the Internet Relay Network");
    client.sendMessage(messageToSend);
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

void Command::privmsg(Message &message)
{

    // isunwoo님이 하신 거
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
    //메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다. 이는
    // 일반적으로 채널이 *[조정되고] 클라이언트가 채널에서 말할 수 있는 권한이
    // 없거나 *[외부 메시지 없음] * 모드가 설정된 채널에 참여하지 않는 등의 채널
    //모드에 대한 응답으로 전송됩니다.

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

    if (receivers[0][0] == '#') // channel로 통신
    {
        std::map<std::string, Channel> &channel = getServerChannel();
        // ERR_NOSUCHSERVER (402) -> 채널이 없음
        for (unsigned int i = 0; i < receivers.size(); i++)
        {
            std::map<std::string, Channel>::iterator iter =
                channel.find(receivers[i].substr(1, receivers[i].size()));
            if (iter == channel.end())
            {
                no_such_server_402(receivers[i].substr(1, receivers[i].size()));
                return;
            }
            // 2. 클라이언트가 채널에 속해있지 않을 때, ERR_CANNOTSENDTOCHAN
            // (404)
            Client &nickname = socketFdToClient[message.getSocket()];
            std::map<std::string, int> member = iter->second.getMembers();
            if (member.find(nickname.getNickname()) == member.end())
            {
                no_member_channel_404(iter->first);
                return;
            }
        }
    }
    // 수신자들 통신
    else
    {
        // ERR_NOSUCHNICK (401) -> 닉네임을 못 찾을 경우
        for (unsigned int i = 0; i < receivers.size(); i++)
        {
            std::map<std::string, int>::iterator iter =
                nicknameToSocketFd.find(receivers[i]);
            if (iter == nicknameToSocketFd.end())
            {
                no_nick_member_401(receivers[i]);
                return;
            }
        }
    }
    // 통신 시작
    for (int i = 0; i < receivers.size(); i++)
    {
        // 채널로 통신
        if (receivers[i][0] == '#') // channel로 통신
        {
            serverInstance->getChannel()[receivers[i]].broadcasting(
                fromNickname, message);
        }
        // 수신자들 통신
        else
        {
            int socketToSend = nicknameToSocketFd[receivers[i]];
            Client &clientToSend = socketFdToClient[socketToSend];
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
    { // const std::string &error_message = ":irc.local  461 * " +
        //                                    message.getCommand() +
        //                                    " :Not enough parameters";
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

    // 1번인 & # + !이 오는지 검사, 길이가 50이하 인지 검사
    if (!(name[0] == '&' || name[0] == '#' || name[0] == '+' ||
          name[0] == '!' || name.size() <= 50))
        return false;

    // 2번 공백 검사, 컨트롤 G 검사, 쉼표 검사 이 3가지가 오면 안 됨
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
    // 채널 이름이 들어가야함. name
    if (message.getArg()[0].empty())
    {
        std::cout << "fuck error\n";
        return;
    }
    std::cout << "Here\n";

    std::map<std::string, Channel> &channel = getServerChannel();
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();

    std::string channel_name = message.getArg()[0];
    std::map<std::string, Channel>::iterator iter = channel.find(channel_name);

    // 채널이 없을 경우 생성
    if (iter == channel.end())
    {
        // JOIN 1번 상황 채널 이름은 최대 50자 길이의 문자열('&', '#', '+' 또는
        // '!' 문자로 시작)입니다.
        if (joinChannelNameCheck(channel_name) == 0)
        {
        }

        Channel newChannel(channel_name);
        newChannel.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 1);
        // this->channel[channel_name] = newChannel;
        channel.insert(make_pair(channel_name, newChannel));

        // 출력해보기
        //  iter = channel.begin();
        //  for ( ; iter != channel.end(); iter++)
        //  {
        //      std::cout << "channel -> channel_name : " <<
        //      iter->first<<std::endl; iter->second.printMember();
        //  }
    }
    // 채널이 있을 경우 그냥 들어가기
    else
    {
        std::cout << "원래 있다 쉐캬\n";
        // invite만 가능한 방
        if (iter->second.getMODE_I())
        {
            // 에러 처리 해주기
            return;
        }
        // 채널이 비밀번호가 있을 때
        if (iter->second.getKey() != "")
        {
            if (message.getArg()[1] == iter->second.getKey())
            {
                iter->second.setMembers(
                    socketFdToClient[message.getSocket()].getNickname(), 0);
            }
            else // 비밀 번호 틀렷을 때 에러 처리
            {
                return;
            }
        }
        iter->second.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 0);
    }
}

void Command::part(Message &message) // cout 한 것 전부 send(error)로 바꿔야!
{
    if (message.getArg()[0].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
        // 추가 에러 넣어줌
        command_empty_argument_461(message);
        return;
    }

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end()) // 나갈 채널이 없을 경우
    {
        // error 403 "<client> <channel> :No such channel"
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 채널은 있는데 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        no_member_channel_442(message);
        return;
    }
    else // 채널 있고, 그 채널 속 유저임 -> 채널 나갈거임 ! 만약 방장이라면 ?..
    {
        if (iterNick->second == 1)
            ; // 방장이라면 : 다른 사람에게 방장 물려주기 / 다른 방장 x시 방
              // 폭파
        members.erase(nickname); // members에서 iterNick 빼기 . .
    }
    return;
}

// KICK <channel> <user> (<reason>) 인데 . . 여러명도 가능이라면 ? 자살
void Command::kick(Message &message)
{
    // 인자 있는지 확인
    std::string a = NULL;

    if (message.getArg()[0].empty() || message.getArg()[1].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
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
        // error 403 "<client> <channel> :No such channel"
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        no_member_channel_442(message);
        return;
    }
    if (iterNick->second != 1) // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        // error 482 "<client> <channel> :You're not channel operator"
        no_operator_channel_482(message);
        return;
    }

    std::string kickName = message.getArg()[1];
    iterNick = members.find(kickName);
    if (iterNick == members.end()) // kick할 사람이 그 채널 속 유저가 아님
    {
        // error 441 "<client> <nick> <channel> :They aren't on that channel"
        no_users_channel_441(message);
        return;
    }

    members.erase(kickName);
    // 관련 메시지 전송
    return;
    /*
        naki
        std::map<std::string, int>::iterator iter = members.find(nickname);

        if (iter == members.end()) // 유저 존재 x
        {
            std::cout << "no nickname" << std::endl;
            return;
        }
        else
        {
            members.erase(nickname);
            // TODO : kick 되었다고 전송?
        }
        return;
    */
}

// TOPIC <channel> (<topic>)
void Command::topic(Message &message)
{
    // 인자 있는지 확인
    if (message.getArg()[0].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
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
        // error 403 "<client> <channel> :No such channel"
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        no_member_channel_442(message);
        return;
    }
    if (iterCh->second.getMODE_T() && iterNick->second != 1)
    // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        // error 482 "<client> <channel> :You're not channel operator"
        no_operator_channel_482(message);
        return;
    }

    // 주제 인자 있는지 확인
    // 없으면 RPL_NOTOPIC (331)
    if (iterCh->second.getTopic().empty())
    {
        no_topic_channel_331(message);
        return;
    }

    // 이거 왜 똑같은 변수가 2개지,,? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::string topic = iterCh->second.getTopic();
    topic = message.getArg()[1]; // 이거 없으면 세그나니까 에러처리 필수
    // 관련 메시지 전송
    return;
}

// INVITE <nickname> <channel>
void Command::invite(Message &message)
{
    // 인자 모두 있는지 확인
    if (message.getArg()[0].empty() || message.getArg()[1].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
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
        // error 403 "<client> <channel> :No such channel"
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        return;
    }
    if (iterNick->second != 1) // 호출한 사람이 채널에 있긴 한데 방장이 아님
    {
        // error 482 "<client> <channel> :You're not channel operator"
        no_member_channel_442(message);
        return;
    }

    std::string newMemberName = message.getArg()[0];
    iterNick = members.find(newMemberName);
    if (iterNick != members.end()) // 초대할 사람이 이미 채널에 있음
    {
        // error 443 : "<client> <nick> <channel> :is already on channel"
        user_already_channel_443(message);
        return;
    }
    iterCh->second.setMembers(newMemberName, 0);
    // 관련 메시지 전송

    /*
        members[nickname] = 0; // 기본 멤버로 초대
    */

    // 341 하면 좋을 듯
    // 341은 에러가 아니라 성공의 메세지를 보냄
    // RPL_INVITING (341)
    success_invite_341(message);
    return;
}

// MODE <channel> +/-<mode> (<param>)
void Command::mode(Message &message)
{
    // 인자 있는지 확인
    if (message.getArg()[0].empty())
    {
        // error 461 "<client> <command> :Not enough parameters"
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
        // error 403 "<client> <channel> :No such channel"
        no_such_channel_403(message);
        return;
    }

    std::string nickname = socketFdToClient[message.getSocket()].getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) // 호출한 사람이 그 채널 속 유저가 아님
    {
        // error 442 "<client> <channel> :You're not on that channel"
        no_member_channel_442(message);
        return;
    }

    // 모드 설정 인자 없이 들어오면 현재 상태 알려주는 듯 !
    if (message.getArg()[1].empty())
    {
        // message 324  "<client> <channel> <modestring> <mode arguments>..."
        state_without_setup_324(message);
        return;
    }

    // 모드 설정 인자 있고, 호출한 사람이 채널에 있긴 한데 방장이 아님
    if (iterNick->second != 1)
    {
        // error 482 "<client> <channel> :You're not channel operator"
        no_operator_channel_482(message);
        return;
    }

    if (message.getArg()[1][1] == 'o')
    {
        if (message.getArg()[2].empty())
        {
            // no param
            return;
        }
        std::string targetName = message.getArg()[2];
        iterNick = members.find(targetName);
        if (iterNick == members.end()) // 설정할 사람이 채널에 없음
        {
            // error
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
        if (message.getArg()[2].empty())
        {
            // no param
            return;
        }
        std::string oldPassword = iterCh->second.getKey();
        std::string newPassword = message.getArg()[2];
        if (message.getArg()[1][0] == '+')
            oldPassword = newPassword;
        else if (message.getArg()[1][0] == '-' && oldPassword == newPassword)
            oldPassword = "";
        else
        {
            // +/- 아니거나, -k인데 패스워드 틀림
            return;
        }
    }

    else if (message.getArg()[1][1] == 'l')
    {
        if (message.getArg()[1][0] == '+' && message.getArg()[2].empty())
        {
            // no param
            return;
        }

        int oldLimit = iterCh->second.getLimit();
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
            // +/- 아님
            return;
        }
    }

    else if (setMode(message, iterCh->second) == false)
    {
        // +/- 아니거나 i/t 아니면 wrong argument
        return;
    }
    // 관련 메시지 전송
    return;
}

bool Command::setMode(Message &message, Channel channel)
{
    bool MODE;

    if (message.getArg()[1][1] == 'i')
        MODE = channel.getMODE_I();
    else if (message.getArg()[1][1] == 't')
        MODE = channel.getMODE_T();
    else
        return false;

    if (message.getArg()[1][0] == '+')
        MODE = true;
    else if (message.getArg()[1][1] == '-')
        MODE = false;
    else
        return false;

    return true;
}

void Command::quit(Message &message)
{
    serverInstance->terminateConnection(message.getSocket());
}