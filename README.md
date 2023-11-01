# ft_irc

## 변경 사항
1. channel class 내 mode bool들 중 필요없는 o랑 k 뺐습니다
비밀번호 유무는 std::string key.empty()로 확인 ㄱ ㄱ
(채널 비밀번호 이름 password -> key로 바꿈)
2. mode l (채널 멤버 수 제한) bool 말고 int로 변수 추가했습니다

## 앞으로 해야할 것

1. 새 채널 JOIN 시 채널명 제한 추가
: Channels names are strings (beginning with a '&', '#', '+' or '!'
   character) of length up to fifty (50) characters.
  it SHALL NOT contain any spaces
   (' '), a control G (^G or ASCII 7), a comma (',' which is used as a
   list item separator by the protocol).
  Also, a colon (':') is used as a delimiter for the channel mask. -> 사용 불가하게 ㄱ ㄱ 
   Channels with '&' as prefix are local to the server where they are created.

   Other channels are known to one (1) or more servers that are
   connected to the network, depending on the channel mask:
-> 채널이름 맨 앞에 붙는 prefix는 &만 가능하게 하고 나머지 금지 시키는거 어떠신지

기존 채널 JOIN 시 MODE_I true, password, member 수 <= limit 확인 필요

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