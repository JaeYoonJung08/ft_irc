# ft_irc

## 앞으로 해야할 것

1. /하고 명령어 끝까지 다 안 입력해도 실행이 되는데요?
2. part나 kick말고 exit으로 나가면 채널 멤버 목록에서 삭제 안됨 -> 다시 초대 안됨 !!
3. invite 성공했는데 채널 화면이 안뜸 ㅜㅜ 메시지는 옴 테스트해보세여
4. 함수 테스트 -> 세그 잡기 중요
5. UPPER_앵무새봇 : /msg parrot I'm a bot! -> reply : I'M A BOT!


테스트 결과
jaeyojun op
naki   

1. kick
    * /kick naki -> 오퍼레이터가 아닌 클라이언트가 kick을 해버리면 채널에서 나가짐
    * /kick naki (reason) -> 클라이언트 강퇴는 되나 강퇴된 클라이언트에서 강퇴되었다는 정보가 따로 뭐가 오지는 않음.
    -> 메시지를 채널 브로드캐스팅으로 보내는거라 강퇴된 사람한텐 안갈걸여

2. part
    * /part -> naki에서 /part를 실행하면 잘 되며 jaeyojun 클라이언트에서도 잘 나옴
    * /part fuck -> naki에서 /part fuck를 실행하면 jaeyojun 클라이언트에서도 잘 나옴 이유도 나옴.

3. invite
    * invite는 성공했지만 채널 화면이 안 뜸. !! 고쳐야될 거
    * 채널에 있는 상태에서 /invite naki -> 메세지가 옴
    * 채널에 있는 상태에서 /invite naki &ch -> 메세지 안 옴 
        * 메세지 오게 만들었음.

4. exit
    * exit으로 나가면 채널 멤버 목록에서 삭제 안 됨.
-> 걍 엑싯 함수를 만들면 되지 아늘까요 ?
-> 만들어 볼게용




2. 

- [ ] authenticate
- [ ] nickname
- [ ] username
- [ ] join a channel
- [ ] private message
- [ ] channel message
- [ ] channel operators
    - [ ] KICK
    - [ ] INVITE
    - [ ] TOPIC
    - [ ] MODE
        - [ ] i
        - [ ] t
        - [ ] k
        - [ ] o
        - [ ] l

- [ ] subject 6쪽 Test example



 
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

## 논의 할 내용

naki : 그리고 인자가 아예 안 들어온 거랑 빈 문자열로 들어온거랑 구분 어케해요?
    + TOPIC 커맨드 RFC 상에는 평소에는 토픽 누구나 설정할 수 있고, mode +t일 때만 오퍼레이터 권한으로 바뀐다고 나와있는데 서브젝트에는 자세히 안 나와있어서 . . 사람들한테 물어보고 진행해야 할듯요 ?
    + 채널 소속 아닌 사람이 모드 확인 가능한지 ? 전 아니라고 생각
    + 아토이 써도돼염?
    + 채널 마스크 사용할지 ? 초대전용모드 정의 확실하게

## 완료 사항

1. Commands
- join
    - jaeyojun -> JOIN 명령어가 채널이 없으면 만들고 채널이 있으면 거기 들어가기 
    - join 명령어 사용 시 없는 채널이면 자동 생성되고 만든 사람이 방장임. 
    - Join 후 반장은 한 명이며, 반장이 반장 권한을 줄 수 있음 by MODE -o
    - 채널명 제한 : 최대 50자, &로 시작, 공백(' '), ^G, 쉼표 x
    - 기존 채널 조인 시 +i, +l, key 확인 추가
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
    - t: 채널 운영자에 대한 TOPIC 명령 제한 설정/제거 // no param
    - k: 채널키(비밀번호) 설정/제거
    - o: 채널 운영자 권한 부여/받기
    - l: 채널에 대한 사용자 제한을 설정/해제 (채널 내 최대 인원 설정한다는 말 !)
- PART
- ping-pong : 일단 이거 했습니다. 적용을 해야할지,,?

## 완료 후 보너스

2. 파일 전송 가능한 클라이언트 선정 && 이론 공부
3. 파란 앵무새 봇

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

콜론(':')은 JOIN 명령어에서 채널 마스크를 구분하는 데 사용되며, 특정 패턴을 정의하여 사용자가 원하는 조건에 맞는 채널에 참여할 수 있도록 도와준다.  

## 4번에 대한 이야기

가정적인 두 개의 IRC 서버, A와 B가 있다고 상상.  

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
    * ERR_ALREADYREGISTERED (462) → 이거 할 필요가 없을 듯
        * 클라이언트가 등록 중에만 설정할 수 있는 세부 정보를 변경하려고 할 때 반환됩니다(예: 등록 후 재전송 PASS) USER. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다양합니다.
        ```
        "<client> :You may not reregister"
        ```
# PRIVMSG 해야됨
4. PRIVMSG  
-> isunwoo님이 해주신 부분은 여러 수신자들한테 보내는 코드가 작성되어 있는 듯하다.  
-> PRIVMSG에서 target자리는 수신자가 될 수도 있고 채널이 될 수도 있는 것 같음.  
-> 이 부분에 대해서 이야기 해봐야할듯.  
-> 이 부분 에러처리는 일단 다음  
    * ERR_CANNOTSENDTOCHAN (404) -> 채널에 못 보낼 경우
        * `PRIVMSG`/ 를 `NOTICE`에 전달할 수 없음 을 나타냅니다 `<channel>`. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.이는 일반적으로 채널이 *[조정되고] 클라이언트가 채널에서 말할 수 있는 권한이 없거나 *[외부 메시지 없음](https://modern.ircdocs.horse/#no-external-messages-mode)* 모드가 설정된 채널에 참여하지 않는 등의 채널 모드에 대한 응답으로 전송됩니다.
        ```
        "<client> <channel> :Cannot send to channel"
        ```
    * ERR_NOSUCHNICK (401) -> 닉네임을 못 찾을 경우
        * 제공된 별명에 대해 클라이언트를 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <nickname> :No such nick/channel"
        ```
    * ERR_NOSUCHSERVER (402) -> 채널이 없음
        * 해당 서버 이름이 존재하지 않음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <server name> :No such server"
        ```
    * ERR_NORECIPIENT (411) -> 수진자 지정 x
        * PRIVMSG수신자가 지정되지 않아 메시지가 배달되지 않았음을 나타내는 명령 에 의해 반환됩니다 .
        ```
        "<client> :No recipient given (<command>)"
        ```
    * ERR_NOTEXTTOSEND (412) -> 보낼 텍스트가 없음.
        * PRIVMSG보낼 텍스트가 없기 때문에 메시지가 배달되지 않았음을 나타내기 위해 명령 에 의해 반환됩니다 .
        ```
        "<client> :No text to send"
        ```
    * RPL_AWAY (301) -> 해당 닉네임 사용자 부재
        * 해당 닉네임의 사용자가 <nick>현재 부재중임을 나타내며, 자신이 설정한 부재중 메시지를 보냅니다.
        ```
        "<client> <nick> :<message>"
        ```
    * • ERR_TOOMANYTARGETS(407) → 이거 있기는 한디 문서에서 안 보임

5. PING 
    * ERR_NEEDMOREPARAMS (461) == 중복
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_NOORIGIN (409) -> 이건 안 해도 될 듯.
        * 이전 IRC 서버에서 요구하는 발신자 매개변수가 누락된 PING 또는 PONG 메시지를 나타냅니다. 요즘에는 PING이 <token>비어 있을 때 일부 서버에서 이 기능을 사용할 수 있습니다.
        ```
        "<client> :No origin specified"
        ```
    * ERR_NOSUCHSERVER (402) == 중복 (이것도 안 해도 될 듯)
        * 해당 서버 이름이 존재하지 않음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <server name> :No such server"
        ```
# JOIN 해야됨
6. JOIN
    * ERR_NEEDMOREPARAMS (461) == 중복 
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_NOSUCHCHANNEL (403) == 중복
        * 제공된 채널 이름에 대해 채널을 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :No such channel"
        ```
    * ERR_TOOMANYCHANNELS (405) -> 이거 안 해도 될 듯?
        * `JOIN`클라이언트가 최대 채널 수에 가입했기 때문에 명령이 실패했음을 나타냅니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다. 
        ```cpp
        "<client> <channel> :You have joined too many channels"
        ```
    * ERR_BADCHANNELKEY (475)
        *  `[JOIN]*채널에 *[키가](https://modern.ircdocs.horse/#key-channel-mode) 필요 하고 키가 올바르지 않거나 제공되지 않았기 때문에 명령이 실패했음을 나타내기 위해 반환되었습니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        * `[ERR_INVALIDKEY](https://modern.ircdocs.horse/#errinvalidkey-525)`*키를 설정할 때 반환될 수 있는 와 혼동하지 마십시오 .
        ```
        "<client> <channel> :Cannot join channel (+k)"
        ```
    * ERR_BANNEDFROMCHAN (474)
        * JOIN클라이언트가 채널에서 금지 되었고 금지 예외가 설정되지 않았기 때문에 명령 이 실패했음을 나타내기 위해 반환되었습니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :Cannot join channel (+b)"
        ```
    * ERR_CHANNELISFULL (471)
        * 클라이언트 제한 모드가 설정되었고 최대 사용자 수가 이미 채널에 참여했기 JOIN때문에 명령이 실패했음을 나타내기 위해 반환되었습니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :Cannot join channel (+l)"
        ```
    * ERR_INVITEONLYCHAN (473)
        * `[JOIN]*채널이 [초대 전용] 모드로 설정되어 있고 클라이언트가 채널에 *[초대되지 않았거나](https://modern.ircdocs.horse/#invite-message) [초대 예외가](https://modern.ircdocs.horse/#invite-exception-channel-mode)* 설정되어 있기 때문에 명령이 실패했음을 나타내기 위해 반환되었습니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :Cannot join channel (+i)"
        ```
    * ERR_BADCHANMASK (476) -> 이건 안해도 될 듯
        * 제공된 채널 이름이 유효하지 않음을 나타냅니다.이는 채널이 존재하지 않지만 유효한 이름일 수 있음을 나타내는 와 유사하지만 더 강력합니다 .
        ```
         "<channel> :Bad Channel Mask"
        ```
    * RPL_TOPIC (332)
        * 채널의 <channel>현재 주제 를 알리기 위해 가입 시 클라이언트에게 전송됩니다 .
        ```
        `"<client> <channel> :<topic>"`
        ``` 
    * RPL_TOPICWHOTIME (333) -> 안 해도 될 듯 
        * <nick>주제를 설정한 사람( )과 주제를 설정한 시기( <setat>Unix 타임스탬프) 를 알려주기 위해 클라이언트에 전송됩니다 . 이후에 전송되었습니다 .RPL_TOPIC (332)
        ```cpp
        "<client> <channel> <nick> <setat>"
        ```
    * RPL_NAMREPLY (353)
        * 명령 에 대한 응답으로 전송된 *`[NAMES](https://modern.ircdocs.horse/#names-message)`*이 숫자는 가입된 클라이언트 `<channel>`와 해당 채널의 상태를 나열합니다. `<symbol>`채널의 상태를 기록합니다. 다음 중 하나일 수 있습니다.
        - `("=", 0x3D)`
            - 공개 채널.
        - `("@", 0x40)`*[비밀 채널 모드](https://modern.ircdocs.horse/#secret-channel-mode)* `"+s"`
            - 비밀 채널().
        - `("*", 0x2A)"+p"`
            - 비공개 채널(이전에는 더 이상 널리 사용되지 않음)
        * `<nick>`해당 채널에 가입된 클라이언트의 별명이며 클라이언트가 채널에 있는 경우 해당 채널에 있는 `<prefix>`가장 높은 *[채널 멤버십 접두사 입니다.](https://modern.ircdocs.horse/#channel-membership-prefixes)* `[prefix]<nick>`이 숫자의 마지막 매개변수는 공백 문자로 구분된 쌍 목록입니다 `(' ', 0x20)`.
        ```
        "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
        ```
    * RPL_ENDOFNAMES (366)  -> 안 해도 될 듯?
        * 명령 에 대한 응답으로 전송되는 NAMES이 숫자는 채널 구성원 이름 목록의 끝을 지정합니다.
        ```
        "<client> <channel> :End of /NAMES list"
        ```


7. PART
    * ERR_PASSWDMISMATCH (464) -> 중복 (이거 여기서 필요한가?)
        * 비밀번호가 올바르지 않거나 제공되지 않아 연결을 등록할 수 없음을 나타내기 위해 반환됩니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> :Password incorrect"
        ``` 
    * ERR_NOSUCHCHANNEL (403) -> 중복
        * 제공된 채널 이름에 대해 채널을 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :No such channel"
        ```
    * ERR_NOTONCHANNEL (442) -> 중복
        * 클라이언트가 자신이 속하지 않은 채널에서 채널에 영향을 미치는 명령을 수행하려고 할 때 반환됩니다.
        ```
        "<client> <channel> :You're not on that channel"
        ```
8. QUIT 
    * -> 이거는 에러에 대한 내용이 없음.
9. EXIT
    * -> 이거 QUIT랑 똑같은 건가..? naki님 필요

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
    * ERR_NEEDMOREPARAMS (461) -> 중복
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_NOSUCHCHANNEL (403) -> 중복
        * 제공된 채널 이름에 대해 채널을 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :No such channel"
        ```
    * RPL_NOTOPIC (331)!! 여기 좀 이상함. 코드 부분에서
        * <channel>채널에 참여할 때 해당 이름의 채널에 설정된 주제가 없음을 알리기 위해 클라이언트에게 전송됩니다 .
        ```
        "<client> <channel> :No topic is set"
        ```
    * RPL_TOPIC (332) -> 이건 에러가 아닌 듯? 
        * 채널의 <channel>현재 주제 를 알리기 위해 가입 시 클라이언트에게 전송됩니다 .
        ```
        "<client> <channel> :<topic>"
        ```
    * RPL_TOPICWHOTIME (333) -> 안 해도 될 듯?
        * <nick>주제를 설정한 사람( )과 주제를 설정한 시기( <setat>Unix 타임스탬프) 를 알려주기 위해 클라이언트에 전송됩니다 . 이후에 전송되었습니다 .RPL_TOPIC (332)
        ```
        "<client> <channel> <nick> <setat>"
        ```

12. INVITE
    * ERR_NEEDMOREPARAMS (461) -> 중복
        * 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없을 때 반환됩니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <command> :Not enough parameters"
        ```
    * ERR_NOSUCHCHANNEL (403) -> 중복
        * 제공된 채널 이름에 대해 채널을 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :No such channel"
        ```
    * ERR_NOTONCHANNEL (442) -> 중복
        * 클라이언트가 자신이 속하지 않은 채널에서 채널에 영향을 미치는 명령을 수행하려고 할 때 반환됩니다.
        ```
        "<client> <channel> :You're not on that channel"
        ```
    * ERR_CHANOPRIVSNEEDED (482) -> 중복
        * 클라이언트에 적절한 채널 권한이 없기 때문에 명령이 실패했음을 나타냅니다 . 이 숫자는 halfop , Operator 등과 같은 다양한 접두사에 적용될 수 있습니다 . 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :You're not channel operator"
        ```
    * RPL_INVITING (341)
        * INVITE시도가 성공했고 해당 별명을 가진 클라이언트가 <nick>에 초대되었음을 나타내기 위해 명령 에 대한 응답으로 전송됩니다 <channel>.
        ```
        "<client> <nick> <channel>"
        ```
    * ERR_USERONCHANNEL (443)
        * 클라이언트가 <nick>이미 가입되어 있는 채널에 초대하려고 하면 반환됩니다.
        ```
        "<client> <nick> <channel> :is already on channel"
        ```

# MODE 해야됨
13. MODE
- 사용자 모드와 채널 모드가 있는데 사용자모드는 안 해도 될 것 같음..? 이야기 해보자
    * <target>네트워크에 존재하지 않는 채널인 경우 숫자가 반환됩니다.
        * ERR_NOSUCHCHANNEL (403) -> 중복
            * <target>네트워크에 존재하지 않는 채널인 경우 숫자가 반환됩니다.
            * 제공된 채널 이름에 대해 채널을 찾을 수 없음을 나타냅니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
            ``` 
            "<client> <channel> :No such channel"
            ```
    * <modestring>제공되지 않으면 숫자 가 반환됩니다. 서버는 현재 모드를 전송할 때 채널 키와 같은 민감한 정보를 숨기도록 선택할 수 있습니다. 서버는 다음과 같은 숫자를 반환할 수도 있습니다 .
        * RPL_CHANNELMODEIS (324)
            * 현재 설정된 채널 모드를 클라이언트에게 알리기 위해 전송됩니다. <channel>채널 이름입니다. 메시지 설명 에 정의된 모드 문자열과 모드 인수(별도의 매개변수로 구분됨)입니다 <modestring>.<mode arguments>MODE
            ```
            "<client> <channel> <modestring> <mode arguments>..."
            ```
        * RPL_CREATIONTIME (329)
            * 채널 생성 시간을 알리기 위해 클라이언트에게 전송됩니다. <channel>채널 이름입니다. <creationtime>네트워크에서 채널이 생성된 시기를 나타내는 유닉스 타임스탬프입니다.
            ```
            "<client> <channel> <creationtime>"
            ```
    * 주어진 경우 <modestring>, 명령을 보내는 사용자는 주어진 모드를 변경하기 위해 대상 채널에 대한 적절한 채널 권한을 가지고 있어야 합니다. 사용자에게 대상 채널의 모드를 변경할 수 있는 적절한 권한이 없으면 서버는 메시지를 처리해서는 안 되며 숫자가 반환됩니다. 사용자에게 대상의 모드 변경 권한이 있는 경우 제공된 모드는 모드 유형에 따라 적용됩니다(아래 참조). A, B, C 유형 모드의 경우 인수는 에서 순차적으로 가져옵니다 . 유형 B 또는 C 모드를 설정할 때 매개변수가 없으면 서버는 해당 모드를 무시해야 합니다. 유형 A 모드가 인수 없이 전송된 경우 목록의 내용은 사용자가 액세스할 수 없는 민감한 정보가 포함되어 있지 않는 한 반드시 사용자에게 전송되어야 합니다. 서버가 모드 처리를 완료하면 모드 변경 사항이 포함된 채널의 모든 구성원에게 명령이 전송됩니다. 서버는 모드 변경을 보낼 때 민감한 정보를 숨기도록 선택할 수 있습니다
        * ERR_CHANOPRIVSNEEDED (482)
        * 클라이언트에 적절한 채널 권한이 없기 때문에 명령이 실패했음을 나타냅니다 . 이 숫자는 halfop , Operator 등과 같은 다양한 접두사에 적용될 수 있습니다. 이 메시지의 마지막 매개변수에 사용된 텍스트는 다를 수 있습니다.
        ```
        "<client> <channel> :You're not channel operator"
        ```