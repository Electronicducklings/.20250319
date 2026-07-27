#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include"login.h"
#include"mainwidget.h"
#include<QTimer>

#define DEAL_FUN_LEN 30


class Kernel : public QObject
{
    Q_OBJECT
public:
    explicit Kernel(QObject *parent = nullptr);
public slots:
    //接收登录框的注册和登录信息的信号
    void slots_login(QString,QString);
    void slots_register(QString,QString,QString);
    //统一接收，所有协议的回复
    void slots_returnClientData(char* ,int,unsigned long);
    //接收聊天的信息和接收信息朋友的id并将这些发给服务端
    void slots_sendMsgToServer(QString msg,int friendid);
    //处理添加好友请求
    void slots_ProtAddFriend(QString nick);
    //定时处理好友下线
    void slots_friOffTimer();
    //定时改变好友信息
    void slots_FriendChangeInformtion();
    //定时请求添加我为好友的信号
    void slots_FriendAddMe();
    //接收来自mainwidget的程序退出消息，由Kernel发给服务端
    void slots_myOfflin();
    //回收客户端资源
    void slots_delete();


signals:
    //模拟服务端，返回给客户端数据的信号
    void signals_returnClientData(char* ,int,unsigned long);

public:
    //处理各种协议的处理函数
    void dealProtRegisterRS(char* ,int,unsigned long);
    //处理登录回复
    void dealProtLoginRS(char* ,int ,unsigned long );
    //处理自己或朋友信息的回复
    void dealProtFriendInfo(char* ,int ,unsigned long);
    //处理聊天回复
    void dealProtChatInfoRs( char* ,int ,unsigned long);
    //处理朋友发信息的请求
    void dealProtFriendChatInfoRq( char* ,int ,unsigned long);
    //处理添加朋友的请求
    void dealPortAddFriend( char* ,int ,unsigned long);
    //处理好友下线业务
    void dealPortFriendOffline( char* ,int ,unsigned long);
    //处理好友改变个人信息
    void dealPortFriendChangeInformtion( char* ,int ,unsigned long);
    //处理定时请求添加我为好友
    void dealPortFriendAddMe(char* ,int ,unsigned long);
public:
    using DealFun=  void (Kernel::*)(char* ,int,unsigned long);

    DealFun m_arrDealFun[DEAL_FUN_LEN];

public:
    Login* m_pLogin; 

   MainWidget* m_pMainWidget;
   QTimer m_friend_OffLine_Timer;          //模拟好友下线的定时器
   QTimer m_friend_Change_information;    //模拟好友改变信息的定时器
   QTimer m_FriendAddMe;                  //模拟请求添加我为好友的定时器
};

#endif // KERNEL_H
