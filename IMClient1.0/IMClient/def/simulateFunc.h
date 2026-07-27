//模拟服务端发送消息

#pragma once

#ifndef __SIMULATEFUNC_h__
#define __SIMULATEFUNC_h__

#include"def.h"
#include"QDebug"
#include"QString"
#include"kernel.h"

#define  RETURN_BUF_DATA(PROT_TYPE,PROT_NAME)\
{\
    /*将回复信息数据打包*/\
    int len =sizeof(PROT_TYPE);\
    char* pBuf=new char[len];\
    memcpy(pBuf,&PROT_NAME,len);\
    /*模拟服务端，返回给客户端数据*/\
    pKernel->signals_returnClientData(pBuf, len,0 );\
}




extern Kernel* pKernel;

//处理注册请求
inline void  simulate_regisert(PROT_register_RQ  registerRq)
{
    qDebug()<<QString("服务端收到的数据有：nick：%1 ,  pass：%2,   tel：%3")
              .arg(registerRq.nick).arg(registerRq.pass).arg(registerRq.tel);

    //服务端要回复的信息
    PROT_register_RS registerRs;

    registerRs.result=Register_Sucess;//虽然有初始值但是我如果想测试失败，所以要重新赋值

    //将回复信息数据打包
   // char* pBuf=new char[20];
   // int len =sizeof(registerRs);
   // memcpy(pBuf,&registerRs,len);

    //模拟服务端，返回给客户端数据
   // pKernel->signals_returnClientData(pBuf, len,0 );
     RETURN_BUF_DATA(PROT_register_RS,registerRs);

}
//处理登录请求
inline void  simulate_login(PROT_LOGIN_RQ loginRq)
{
    qDebug()<<" simulate_login 服务端收到登录请求 ";
    qDebug()<<QString("服务端收到的数据有:  pass：%1, tel：%2").
              arg(loginRq.pass).arg(loginRq.tel);

    //服务端回复登录的信息
    PROT_LOGIN_RS  loginRs;
    loginRs.result=LOGIN_Sucess;
    loginRs.userid=5;//主用户

     //将回复信息数据打包
   // char* pBuf=new char[20];
    //int len=sizeof(PROT_LOGIN_RS );
   // memcpy(pBuf,&loginRs,len);

    //服务端发送数据包给客户端
    //pKernel->signals_returnClientData(pBuf,len,0);
     RETURN_BUF_DATA( PROT_LOGIN_RS,loginRs);

    //如果判断登录成功，之后，需要查询客户端的信息，头像，昵称，签名，并返回给客户端
    if(LOGIN_Sucess==loginRs.result)
    {

        //主用户信息
        PROT_FRIEND_INFO userInfo;
         qDebug()<<"服务器主用户："<<userInfo.iconid;
        userInfo.userid=5;
        userInfo.status=ONLINE;
        userInfo.prottype=DEF_PROT_FRIEND_INFO;
        strcpy_s(userInfo.nick,30,"徐佑恒");
        strcpy_s(userInfo.feeling,100,"有点喜欢丘~");

        RETURN_BUF_DATA(  PROT_FRIEND_INFO ,userInfo);

        //朋友信息
        PROT_FRIEND_INFO friend1;
         qDebug()<<"服务器朋友1："<<friend1.iconid;
        friend1.userid=12;
        friend1.iconid=12;
        friend1.status=ONLINE;
        friend1.prottype=DEF_PROT_FRIEND_INFO;
        strcpy_s(friend1.nick,30,"刘文丘");
        strcpy_s(friend1.feeling,100,"他喜欢我哦~");

        RETURN_BUF_DATA(  PROT_FRIEND_INFO ,friend1);


        PROT_FRIEND_INFO friend2;
         qDebug()<<"服务器朋友2："<<friend2.iconid;
        friend2.userid=11;
        friend2.iconid=11;
        friend2.status=ONLINE;
        friend2.prottype=DEF_PROT_FRIEND_INFO;
        strcpy_s(friend2.nick,30,"徐思琪");
        strcpy_s(friend2.feeling,100,"她也喜欢我哦~");

        RETURN_BUF_DATA(  PROT_FRIEND_INFO ,friend2);


        PROT_FRIEND_INFO friend3;
         qDebug()<<"服务器朋友3："<<friend3.iconid;
        friend3.userid=15;
        friend3.iconid=15;
        friend3.status=OFFLINE;
        friend3.prottype=DEF_PROT_FRIEND_INFO;
        strcpy_s(friend3.nick,30,"郑嘉晖");
        strcpy_s(friend3.feeling,100,"他把我甩了");

        RETURN_BUF_DATA(  PROT_FRIEND_INFO ,friend3);


        PROT_FRIEND_INFO friend4;
        friend4.userid=20;
        friend4.iconid=20;
        friend4.status=OFFLINE;
        friend4.prottype=DEF_PROT_FRIEND_INFO;
        strcpy_s( friend4.nick,30,"林泽涛");
        strcpy_s( friend4.feeling,100,"他跟我失联了");

        RETURN_BUF_DATA(  PROT_FRIEND_INFO ,friend4);

    }



}
//处理聊天请求
inline void  simulate_chat(PROT_Chat_INFO_RQ chatRq)
{
    qDebug()<<QString("服务端收到了来自 %1 发送给 %2 的聊天请求").arg(chatRq.senderId).arg(chatRq.receiverId);

  //偷个懒，只认为刘文丘在线，模拟回复
 if(12==chatRq.receiverId)
 {
    PROT_Chat_INFO_RS chatRs;
    chatRs.senderId=12;
    chatRs.receiverId=5;
    chatRs.result=Chat_RESULT_SUCCESS ;
    chatRs.prottype=DEF_PROT_Chat_INFO_RS ;
     RETURN_BUF_DATA(  PROT_Chat_INFO_RS ,chatRs);

     //模拟朋友向我们发送消息请求
     PROT_Chat_INFO_RQ friendChatRq;
     friendChatRq.senderId=chatRq.receiverId;
     friendChatRq.receiverId=chatRq.senderId;
     strcpy_s( friendChatRq.msg  ,msgLenth ,   "你好鸭，佑恒 ~，真的......很高兴认识你，月色真美啊！" );
     RETURN_BUF_DATA(    PROT_Chat_INFO_RQ  , friendChatRq);

 }else{
     PROT_Chat_INFO_RS chatRs;
     chatRs.senderId=chatRq.receiverId;
     chatRs.receiverId=chatRq.senderId;
     chatRs.result=Chat_RESULT_FAIL ;
     chatRs.prottype=DEF_PROT_Chat_INFO_RS ;
     RETURN_BUF_DATA(  PROT_Chat_INFO_RS ,chatRs);
 }



}
//处理添加好友请求
inline void simulate_addFriend(PROT_ADD_FRIEND_RQ addFriendRq)
{
     qDebug()<<QString("服务端simulate收到了%1请求添加%2为好友的请求").arg(addFriendRq.senderNick)
               .arg(addFriendRq.receiverNick);
     //  加好友


     if("小明"==QString(addFriendRq.receiverNick))
     {
        //加小明 ，在线，且同意
         qDebug()<<"进小明的函数了";
        PROT_ADD_FRIEND_RS  addFriendRs;
        addFriendRs.senderId=13;
        strcpy_s(addFriendRs.senderNick,30,"小明");
        addFriendRs.receiverId=5;
        strcpy_s(addFriendRs.receiverNick,30,"徐佑恒");
        addFriendRs.result=ADDFRIEND_AGREE;
        RETURN_BUF_DATA(   PROT_ADD_FRIEND_RS  ,addFriendRs);

        //返回小明的信息给徐佑恒
        PROT_FRIEND_INFO  friendInformation;
        strcpy_s(friendInformation.feeling,100,"浪迹天涯");
        friendInformation.iconid=13;
        strcpy_s(friendInformation.nick,30,"小明");
        friendInformation.status=ONLINE ;
        friendInformation.userid=13;
        RETURN_BUF_DATA(    PROT_FRIEND_INFO  ,friendInformation);


     }else if("张三"==QString(addFriendRq.receiverNick)){
        //加张三 ，在线，且拒绝
        PROT_ADD_FRIEND_RS  addFriendRs;
        addFriendRs.senderId=14;
        strcpy_s(addFriendRs.senderNick,30,"张三");
        addFriendRs.receiverId=5;
        strcpy_s(addFriendRs.receiverNick,30,"徐佑恒");
        addFriendRs.result=ADDFRIEND_REJECT;
        RETURN_BUF_DATA(   PROT_ADD_FRIEND_RS  ,addFriendRs);

     }else if("王五"==QString(addFriendRq.receiverNick)){
         //加王五 ，离线，
         PROT_ADD_FRIEND_RS  addFriendRs;
         addFriendRs.senderId=18;
         strcpy_s(addFriendRs.senderNick,30,"王五");
         addFriendRs.receiverId=5;
         strcpy_s(addFriendRs.receiverNick,30,"徐佑恒");
         addFriendRs.result=ADDFRIEND_OFFLINE ;
         RETURN_BUF_DATA(   PROT_ADD_FRIEND_RS  ,addFriendRs);
     }else{
         //用户不存在
          qDebug()<<"进用户不存在函数了";
          PROT_ADD_FRIEND_RS  addFriendRs;
          addFriendRs.result=ADDFRIEND_USER_NOEXIT;
          RETURN_BUF_DATA(   PROT_ADD_FRIEND_RS  ,addFriendRs);
     }



}

//处理用户下线
inline void UerOffline(PROT_OFFLINE userInfo)
{
    qDebug()<<QString("服务端收到%1的下线通知").arg(userInfo.friendId);

    //通知徐佑恒的每个朋友
}

//
inline void simu_addFriendRs( PROT_ADD_FRIEND_RS addFriRs)
{
    qDebug()<<QString("服务端收到%1给%2的添加好友的回复").arg(addFriRs.receiverNick).arg(addFriRs.senderNick);
    if( ADDFRIEND_AGREE    ==addFriRs.result)
    {
        PROT_FRIEND_INFO friInfo;
        friInfo.userid=25;
        friInfo.iconid=25;
        friInfo.status=ONLINE  ;
        strcpy_s(friInfo.nick,30,"老李");
        strcpy_s(friInfo.feeling,100,"6666666666666");
        RETURN_BUF_DATA(   PROT_FRIEND_INFO  ,friInfo);

    }else if( ADDFRIEND_REJECT     ==addFriRs.result){
        qDebug()<<"很遗憾，";

    }
}



#endif
