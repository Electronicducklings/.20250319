#pragma once

#ifndef __DEF_h__
#define __DEF_h__

#define  PROT_BASE 1000
//注册请求的协议类型
#define DEF_PROT_REGISTER_RQ     (PROT_BASE+0)
//注册时服务端回复的协议类型
#define DEF_PROT_REGISTER_RS     (PROT_BASE +1)
//登录请求的协议类型
#define DEF_PROT_LOGIN_RQ        (PROT_BASE+2)
//登录回复协议类型
#define DEF_PROT_LOGIN_RS        (PROT_BASE+3)
//用户信息的协议类型
#define DEF_PROT_FRIEND_INFO     (PROT_BASE+4)
//聊天请求的协议类型
#define DEF_PROT_Chat_INFO_RQ    (PROT_BASE+5)
//聊天回复的协议类型
#define DEF_PROT_Chat_INFO_RS    (PROT_BASE+6)
//添加好友请求的协议类型
#define DEF_PROT_ADD_FRIEND_RQ   (PROT_BASE+7)
//添加好友回复的协议类型
#define DEF_PROT_ADD_FRIEND_RS   (PROT_BASE+8)
//处理好友下线的协议类型
#define DEF_PROT_OFFLINE         (PROT_BASE+9)


//服务端回复注册状态
#define Register_Sucess 1
#define Register_Fail   0

//服务端回复登录状态
#define LOGIN_Sucess    1
#define NOEXIT          2
#define PASSERR         3

//用户状态
#define ONLINE          1
#define OFFLINE         0

//聊天发送信息的长度
#define msgLenth      1024*8

//聊天请求成功与否
#define Chat_RESULT_SUCCESS      1
#define Chat_RESULT_FAIL         0

//添加好友情况：接受，拒绝， 离线， 用户不存在
#define ADDFRIEND_AGREE          1
#define ADDFRIEND_REJECT         2
#define ADDFRIEND_OFFLINE        3
#define ADDFRIEND_USER_NOEXIT    4



using ProtType= unsigned int;

//客户端注册请求的协议
struct PROT_register_RQ
{
    ProtType prottype;//协议类型
    char nick[30];//昵称
    char tel[30];//手机号
    char pass[30];//密码
    PROT_register_RQ():prottype(DEF_PROT_REGISTER_RQ),nick{0},tel{0},pass{0}
   {}

};

//注册时服务端回复的协议
struct PROT_register_RS
{
    ProtType prottype;//协议类型
    int result;//
    PROT_register_RS():prottype(DEF_PROT_REGISTER_RS),result( Register_Sucess )
   {}

};

//客户端登录时请求的协议
struct PROT_LOGIN_RQ
{
   ProtType prottype;
   char tel[30];
   char pass[30];
   PROT_LOGIN_RQ():prottype(DEF_PROT_LOGIN_RQ),tel{0},pass{0}
   {}
};

//注册时服务端回复的协议
struct PROT_LOGIN_RS
{
    ProtType  prottype;
    int userid;
    int result;
    PROT_LOGIN_RS():prottype(DEF_PROT_LOGIN_RS ),userid(0),result(LOGIN_Sucess)
    {}

};

//用户信息
struct PROT_FRIEND_INFO
{
     ProtType  prottype;
     int userid;              //用户图标id
     int iconid;              //用户图标id
     int status;              //用户状态
     char feeling[100];       //个性签名
     char nick[30];           //用户昵称
     PROT_FRIEND_INFO():prottype(DEF_PROT_FRIEND_INFO),userid(0),iconid(0),status(ONLINE),feeling{0},nick{0}
     {}
};

//聊天请求
struct  PROT_Chat_INFO_RQ{
    ProtType prottype;
    int senderId;           //发送聊天信息的人的id
    int receiverId ;      //接收聊天信息的人的id
    char msg[msgLenth];           //聊天内容（长度8k）
    PROT_Chat_INFO_RQ(): prottype(DEF_PROT_Chat_INFO_RQ),senderId(0),receiverId(0),msg{0}
     {}
};

//聊天回复
struct  PROT_Chat_INFO_RS{
    ProtType prottype;
    int senderId;           //发送聊天信息的人的id
    int receiverId ;      //接收聊天信息的人的id
    int result;           //聊天请求是否请求成功
    PROT_Chat_INFO_RS(): prottype(DEF_PROT_Chat_INFO_RS),senderId(0),receiverId(0),result(Chat_RESULT_SUCCESS)
     {}
};

//添加好友请求的协议类型
struct PROT_ADD_FRIEND_RQ{
     ProtType prottype;
     int senderId;
     char senderNick[30];
     char receiverNick[30];
     PROT_ADD_FRIEND_RQ():prottype(DEF_PROT_ADD_FRIEND_RQ ),senderId(0),senderNick{0},receiverNick{0}
     {}
};

//添加好友回复的协议类型
struct PROT_ADD_FRIEND_RS {
    ProtType prottype;
    int result;                //结果
    int receiverId;           //回复目标id
    char receiverNick[30];   //回复目标昵称
    char senderNick[30]  ;   //发送者的昵称
    int  senderId;           //发送者的Id
    PROT_ADD_FRIEND_RS(): prottype(DEF_PROT_ADD_FRIEND_RS),result(ADDFRIEND_AGREE),receiverId(0),receiverNick{0}
      ,senderNick{0}, senderId(0)
    {}
};

//处理好友下线的协议类型
struct PROT_OFFLINE{
     ProtType prottype;
     int friendId;
     PROT_OFFLINE():prottype(DEF_PROT_OFFLINE),friendId(0)
     {}
};

#endif
