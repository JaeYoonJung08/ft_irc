# ft_irc

## 변경 사항
1. channel class 내 mode bool들 중 필요없는 o랑 k 뺐습니다
비밀번호 유무는 std::string key.empty()로 확인 ㄱ ㄱ
(채널 비밀번호 이름 password -> key로 바꿈)
2. mode l (채널 멤버 수 제한) bool 말고 int로 변수 추가했습니다

## 앞으로 해야할 것

# 새 채널 JOIN 시 채널명 제한 추가
<br>
1. Channels names are strings (beginning with a '&', '#', '+' or '!'
   character) of length up to fifty (50) characters.

    번역 : 채널 이름은 최대 50자 길이의 문자열('&', '#', '+' 또는 '!' 문자로 시작)입니다. 

<br>
2. it SHALL NOT contain any spaces (' '), a control G (^G or ASCII 7), a comma (',' which is used as a list item separator by the protocol).

    번역 : 여기에는 공백(' '), 컨트롤 G(^G 또는 ASCII 7), 쉼표(',' 프로토콜에 의해 목록 항목 구분 기호로 사용됨)가 포함되어서는 안 됩니다.
<br>
  3. Also, a colon (':') is used as a delimiter for the channel mask. -> 사용 불가하게 ㄱ ㄱ\
  
    번역 : 또한 채널 마스크의 구분 기호로 콜론(':')이 사용됩니다.
<br>
  4. Channels with '&' as prefix are local to the server where they are created.

    번역 : 접두사로 '&'가 있는 채널은 해당 채널이 생성된 서버에 대해 로컬입니다.

<br>
   5. Other channels are known to one (1) or more servers that are
   connected to the network, depending on the channel mask:

    번역 : 다른 채널은 채널 마스크에 따라 네트워크에 연결된 하나 이상의 서버에 알려져 있습니다.
    -> 채널이름 맨 앞에 붙는 prefix는 &만 가능하게 하고 나머지 금지 시키는거 어떠신지

### 기존 채널 JOIN 시 MODE_I true, password, member 수 <= limit 확인 필요

<br>

JOIN 후 채널 정보 메시지 전송
Once a user has joined a channel, he receives information about
   all commands his server receives affecting the channel.  This
   includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
   This allows channel members to keep track of the other channel
   members, as well as channel modes.

2. Reply
    -  PASS, NICK, USER 후에 001 welcome.. 뭐시기 보내줘야함
    -  PASS ERROR 등등 노션에 정리해두심

3. Client별 buffer 처리
    - 데이터가 한 번에 안들어 오고 조금씩 들어올 때 CR-LF ("\r\n") 이 들어오기 전까지는 처리하지말고 들고 있어야함.

4. 각종 error 처리

5. 데이터 주고 받을 시
    -  send도 바로 보내지말고 kqueue릍 통해서 보내야함
    -  write 할 수 있을 때

## 논의 할 내용

naki : 방장이 채널 떠나면 PART커맨드에 주석 적어둔 경우 중에 뭘로 할지 정해야댐

    + 제가만든 함수들 전부 공통 에러처리 넣어놨는데 다 너무 중복되니까 가능한 건 message class에서 처리하는 걸로 옮기는 게 좋을듯

    + 그리고 인자가 아예 안 들어온 거랑 빈 문자열로 들어온거랑 구분 어케해요?

    + 커맨드 함수들 너무 많은데 파일로 따로 빼거나 아예 파일 하나당 함수 하나씩 해서 폴더를 만들죠.. 
    혹시 메소드는 따로 못 빼나요?? 그렇다면 싱글톤방식 ㄱ ㄱ 
    
    + TOPIC 커맨드 RFC 상에는 평소에는 토픽 누구나 설정할 수 있고, mode +t일 때만 오퍼레이터 권한으로 바뀐다고 나와있는데 서브젝트에는 자세히 안 나와있어서 . . 사람들한테 물어보고 진행해야 할듯요 ?

    + 채널 소속 아닌 사람이 모드 확인 가능한지 ? 전 아니라고 생각

    + 아토이 써도돼염?

jaeyojun : OP 명령어가 그 채널의 방장을 말하는 게 아니라 그냥 서버의 운영자를 말하는 것 같음.
    - naki : ㅇㅈ 

## 완료 사항

1. Commands
- join
    - jaeyojun -> JOIN 명령어가 채널이 없으면 만들고 채널이 있으면 거기 들어가기 
    - join 명령어 사용 시 없는 채널이면 자동 생성되고 만든 사람이 방장임. 
    - Join 후 반장은 한 명이며, 반장이 반장 권한을 줄 수 있음 by MODE -o
- kick
    - Channel 클래스에 있는 사람 내보내기
    members.erase(kickName);
-  invite
    - Channel 클래스에 nickname을 통해 사람 초대하기
    iterCh->second.setMembers(newMemberName, 0);
-  topic
    - Channel 방주제 설정
    topic = message.getArg()[1];
-  mode
    - i: 초대 전용 채널 설정/제거 // no param
    For channels which have the invite-only flag set (See Section 4.2.2
   (Invite Only Flag)), users whose address matches an invitation mask
   set for the channel are allowed to join the channel without any
   invitation.
    - t: 채널 운영자에 대한 TOPIC 명령 제한 설정/제거 // no param
    - k: 채널키(비밀번호) 설정/제거
    - o: 채널 운영자 권한 부여/받기
    - l: 채널에 대한 사용자 제한을 설정/해제 (채널 내 최대 인원 설정한다는 말 !)

- PART : 채널 떠나는 역할인데 server에서 하실지 channel에서 하실지 ?
    - isunwoo -> 채널에서 하면 될듯?
    - naki -> 커맨드 보낸 사람 소켓 확인 필요해서 서버에서 햇습니다..

- ping-pong : 일단 이거 했습니다. 적용을 해야할지,,?

## 완료 후 보너스

2. 파일 전송 가능한 클라이언트 선정 && 이론 공부

3. 파란 앵무새 봇


<br>

# 11_02 jaeyojun 
## 1. JOIN 채널 이름 안 되는 거 조사랑 코딩
naki님이 조사하신 것들은 -> 여기 있는 듯 RFC 2811 : Channel Management  
하나씩 해보자

<br>

### 1번 완료
1. Channels names are strings (beginning with a '&', '#', '+' or '!'
   character) of length up to fifty (50) characters.

    번역 : 채널 이름은 최대 50자 길이의 문자열('&', '#', '+' 또는 '!' 문자로 시작)입니다. 

<br>

### 2번 들어가기 전에 간단 설명

<br>
contorl G에 대한 이야기  
 
프로세스 관리: 프로세스를 중지한 후, 다른 작업을 실행하거나 필요에 따라 새로운 명령을 입력할 수 있다.

macOS의 터미널에서 Ctrl+G를 사용하여 프로세스를 중지하는 간단한 예제입니다. 이와 같은 명령은 일반적으로 현재 실행 중인 명령을 중지하고 다른 명령을 입력하도록 터미널을 활성화함. 또한 또 다른 이름이 있는데 bell 이라고도 함. 프로세스를 종료하면 벨 소리가 남.

프로세스 종료 예제

Ctrl + r
history 명령어도 좋기는 하지만, 생각나는 글자들로 찾기에는 역시!

(reverse-i-search)`': (찾고자 하는 글자들로 입력)
Ctrl + R 또 누르면 또 찾아줍니다!  
Ctrl + O를 누르면 해당 명령어를 실행해 줍니다!  
Ctrl + G를 누르면 그냥 명령어를 실행 안 하고 빠져나옵니다! 

https://stepbystep1.tistory.com/10 -> 여기서 Ctrl + g 아스키코드값 확인.
Ctrl-G BEL	7 -> 아스키 코드 값

<br>

### 2번 완료
2. it SHALL NOT contain any spaces (' '), a control G (^G or ASCII 7), a comma (',' which is used as a list item separator by the protocol).

    번역 : 여기에는 공백(' '), 컨트롤 G(^G 또는 ASCII 7), 쉼표(',' 프로토콜에 의해 목록 항목 구분 기호로 사용됨)가 포함되어서는 안 됩니다.


<br>
---


## 3,4번 들어가기 전에 간단 설명

### 콜론에 대한 이야기 -> naki님은 이걸 사용 불가능하게 만들고 싶다고 이야기 하신 것 같음
IRC에서 JOIN 명령어는 사용자가 채널에 참여하는 명령어이며, 채널 마스크는 특정 패턴 또는 규칙에 따라 채널을 선택하거나 식별하기 위해 사용됨.  

JOIN 명령어에서 채널 마스크를 정의할 때 콜론(':')은 종종 사용됨. 이 콜론은 채널 마스크를 표시하고 다른 JOIN 명령어의 매개변수와 구분하는 데 사용됨.  

예를 들어, JOIN 명령어에서 채널 마스크를 사용하여 특정 패턴의 채널에 참여할 수 있습니다. 일반적으로 채널 이름에 와일드카드를 사용하거나 특정 패턴을 지정할 때 채널 마스크를 활용한다.  

JOIN 명령어의 구조  

bash 기준  
  
JOIN #채널이름 [:채널마스크]  

#채널이름: 입장하려는 특정 채널의 이름  
[:채널마스크]: 선택적으로, 채널 마스크를 사용하여 특정 패턴의 채널을 지정할 수 있음
채널 마스크에서 콜론(':')을 사용하면 특정 패턴을 나타내며, 이를 통해 사용자는 특정 조건에 맞는 채널에 참여할 수 있습니다. 예를 들어:  

#engineering:* : 모든 이름이 'engineering'으로 시작하는 채널  
#project?:secret : 'projectA' 또는 'projectB'와 같은 채널  
#fun-convo: : 모든 채널 이름이 'fun-convo'인 채널  


<br>
콜론(':')은 JOIN 명령어에서 채널 마스크를 구분하는 데 사용되며, 특정 패턴을 정의하여 사용자가 원하는 조건에 맞는 채널에 참여할 수 있도록 도와준다.  


<br>

## 4번에 대한 이야기

가정적인 두 개의 IRC 서버, A와 B가 있다고 상상.  


<br>

로컬 채널 (& 접두사가 있는 채널):  

사용자가 서버 A에 연결되어 있습니다. 서버 A에서 새로운 채널을 만듭니다. 새 채널 이름은 "&localchannel" 입니다. 사용자가 &localchannel 채널에 들어가면:  
  
JOIN &#localchannel  
이 채널은 서버 A에서 생성되었으며, 사용자가 현재 서버 A에 연결되어 있기 때문에 사용자는 &localchannel 채널에 참여할 수 있습니다. 이 채널은 서버 A에서만 유효합니다.  
  


<br>
전역 채널 (# 접두사가 있는 채널):  
  
이번에는 사용자가 서버 B에 연결되어 있습니다. 서버 B에서 새로운 채널을 생성합니다. 새 채널 이름은 "#globalchannel" 입니다. 사용자가 #globalchannel 채널에 들어가면:  
  

<br>
JOIN #globalchannel  
이 채널은 서버 B에서 생성되었고, 사용자가 현재 서버 B에 연결되어 있으므로 사용자는 #globalchannel 채널에 참여할 수 있습니다. 이 채널은 서버 B에서 생성되었지만 IRC 네트워크 내에서 다른 서버들과 공유됩니다.  

<br>
따라서, '&' 접두사가 있는 채널은 해당 채널이 생성된 서버에 로컬하며, 사용자가 연결된 서버에서만 유효합니다. 반면에, '#' 접두사가 있는 채널은 해당 채널이 생성된 서버와는 상관없이 IRC 네트워크 전체에서 공유되는 전역 채널입니다.


## 3번 4번은 한 번 이야기 해봐야할 듯

<br>
  3. Also, a colon (':') is used as a delimiter for the channel mask. -> 사용 불가하게 ㄱ ㄱ
  
    번역 : 또한 채널 마스크의 구분 기호로 콜론(':')이 사용됩니다.

<br>
  4. Channels with '&' as prefix are local to the server where they are created.

    번역 : 접두사로 '&'가 있는 채널은 해당 채널이 생성된 서버에 대해 로컬입니다.

<br>
   5. Other channels are known to one (1) or more servers that are
   connected to the network, depending on the channel mask:

    번역 : 다른 채널은 채널 마스크에 따라 네트워크에 연결된 하나 이상의 서버에 알려져 있습니다.
    -> 채널이름 맨 앞에 붙는 prefix는 &만 가능하게 하고 나머지 금지 시키는거 어떠신지

<br>

# JOIN 명령어 정리 RFC 2813 

4.2.1 Join message

      Command: JOIN
   Parameters: <channel>[ %x7 <modes> ]
               *( "," <channel>[ %x7 <modes> ] )

   The JOIN command is used by client to start listening a specific channel. Whether or not a client is allowed to join a channel is checked only by the local server the client is connected to; all other servers automatically add the user to the channel when the command is received from other servers.

   Optionally, the user status (channel modes 'O', 'o', and 'v') on the
   channel may be appended to the channel name using a control G (^G or ASCII 7) as separator.  Such data MUST be ignored if the message wasn't received from a server.  This format MUST NOT be sent to clients, it can only be used between servers and SHOULD be avoided.





Kalt                         Informational                     [Page 14]

RFC 2813          Internet Relay Chat: Server Protocol        April 2000


   The JOIN command MUST be broadcast to all servers so that each server knows where to find the users who are on the channel.  This allows optimal delivery of PRIVMSG and NOTICE messages to the channel.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL               ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
           RPL_TOPIC

   Examples:

   :WiZ JOIN #Twilight_zone        ; JOIN message from WiZ

<br>
# 번역

4.2.1 가입 메시지

       명령: JOIN
    매개변수: <채널>[ %x7 <모드> ]
                *( "," <채널>[ %x7 <모드> ] )

    JOIN 명령은 클라이언트가 특정 채널 청취를 시작하는 데 사용됩니다. 클라이언트의 채널 가입 허용 여부는 클라이언트가 연결된 로컬 서버에서만 확인됩니다. 다른 모든 서버는 다른 서버로부터 명령이 수신되면 자동으로 사용자를 채널에 추가합니다.

    선택적으로 사용자 상태(채널 모드 'O', 'o' 및 'v')는
    채널은 구분 기호로 컨트롤 G(^G 또는 ASCII 7)를 사용하여 채널 이름에 추가될 수 있습니다. 메시지가 서버에서 수신되지 않은 경우 이러한 데이터는 무시되어야 합니다. 이 형식은 클라이언트로 전송되어서는 안 되며, 서버 간에만 사용할 수 있으므로 피해야 합니다.





Kalt 정보 [페이지 14]

RFC 2813 인터넷 릴레이 채팅: 서버 프로토콜 2000년 4월


    JOIN 명령은 각 서버가 채널에 있는 사용자를 찾을 수 있는 위치를 알 수 있도록 모든 서버에 브로드캐스트되어야 합니다. 이를 통해 PRIVMSG 및 NOTICE 메시지를 채널에 최적으로 전달할 수 있습니다.

    숫자 답장:

            ERR_NEEDMOREPARAMS ERR_BANNEDFROMCHAN
            ERR_INVITEONLYCHAN ERR_BADCHANNELKEY
            ERR_CHANNELISFULL ERR_BADCHANMASK
            ERR_NOSUCHCHANNEL ERR_TOOMANYCHANNELS
            ERR_TOOMANYTARGETS ERR_UNAVAILRESOURCE
            RPL_TOPIC

    예:

    :WiZ JOIN #Twilight_zone ; WiZ의 JOIN 메시지

<br>

## 2. 명령어에 대한 에러 처리 해보자.
1. PASS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* 이미 소켓이 Server에 들어온 상황,, client도 만들어짐 그래서 PASS를 할 떄 설정할 수 있는 방법 2가지
    * 1. Server에서 map 컨테이너를 만들어서 인증이 된 소켓(Client)만 만들어진 map에 넣는다..
    * 2. map을 만들지말고 NICK, USER를 설정하지 못 하게 만드는 방법 -> 이 방법이 쉽긴 할 듯
    * 3. 비밀번호 틀리면 클라이언트 exit..?
    * ERR_PASSWDMISMATCH (464)
        * 비밀번호가 올바르지 않거나 제공되지 않아 연결을 등록할 수 없음을 나타내기 위해 반환됩니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<clint> :Password incorrect" 
        ```
    * ERR_NEEDMOREPARAMS (461)
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_ALREADYREGISTERED (462) -> 이런 거까지 해야되나,,?
        * 이거 뭔지 잘 모르겠음 
        * 클라이언트가 등록 중에만 설정할 수 있는 세부 정보를 변경하려고 할 때 반환됩니다(예: 등록 후 재전송 PASS) USER. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다양합니다.

2. NICK 
    * ERR_NICKNAMEINUSE (433) 
        * NICK원하는 별명이 이미 네트워크에서 사용 중이므로 명령을 성공적으로 완료할 수 없는 경우 반환됩니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <nick> :Nickname is already in use"
        ```
    * ERR_NONICKNAMEGIVEN (431) 
        * 명령에 닉네임 매개변수가 필요하지만 제공되지 않은 경우 반환됩니다.
        ```
        "<client> :No nickname given"
        ```
    * ERR_ERRONEUSNICKNAME (432)
        * NICK원하는 별명에 서버에서 허용하지 않는 문자가 포함되어 있어 명령을 성공적으로 완료할 수 없는 경우 반환됩니다 . 다양한 IRC 서버에서 허용되는 문자에 대한 자세한 내용은 NICK명령을 참조하세요 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <nick> :Erroneus nickname"
        ```
    * ERR_NICKCOLLISION (436)
        * 닉네임 충돌(다른 서버에 이미 존재하는 NICK에 등록됨)이 감지되면 서버가 클라이언트에 반환합니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <nick> :Nickname collision KILL from <user>@<host>"
        ```

3. USER
    * ERR_NEEDMOREPARAMS (461)
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_ALREADYREGISTERED (462)
        * 클라이언트가 등록 중에만 설정할 수 있는 세부 정보를 변경하려고 할 때 반환됩니다(예: 등록 후 재전송 PASS) USER. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다양합니다.
        ```
        "<client> :You may not reregister"
        ```
4. PRIVMSG
5. PING
6. JOIN
7. PART
8. QUIT
9. EXIT
10. KICK
    * ERR_NEEDMOREPARAMS (461)
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_NOSUCHCHANNEL (403)
        * 제공된 채널 이름에 대해 채널을 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :No such channel"
        ```
    * ERR_NOTONCHANNEL (442)
        * 클라이언트가 자신이 속하지 않은 채널에서 채널에 영향을 미치는 명령을 수행하려고 할 때 반환됩니다.
        ```
        "<client> <channel> :You're not on that channel"
        ```
    * ERR_CHANOPRIVSNEEDED (482)
        * 클라이언트에 적절한 채널 권한이 없기 때문에 명령이 실패했음을 나타냅니다 . 이 숫자는 halfop , Operator 등과 같은 다양한 접두사에 적용될 수 있습니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :You're not channel operator"
        ```
    * ERR_USERNOTINCHANNEL (441)
        * 클라이언트가 채널+닉네임에 영향을 주는 명령을 수행하려고 시도할 때, 닉네임이 채널에 연결되지 않은 경우 반환됩니다(예: MODE #channel +o nick).
        ```
        "<client> <nick> <channel> :They aren't on that channel"
        ```
    * ERR_BADCHANMASK (476) -> 이거 안 했음 
        * 제공된 채널 이름이 유효하지 않음을 나타냅니다.이는 채널이 존재하지 않지만 유효한 이름일 수 있음을 나타내는 와 유사하지만 더 강력합니다 .*`[ERR_NOSUCHCHANNEL](https://modern.ircdocs.horse/#errnosuchchannel-403)`* `(403)` 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<channel> :Bad Channel Mask"
        ```
11. TOPIC
12. INVITE
13. MODE