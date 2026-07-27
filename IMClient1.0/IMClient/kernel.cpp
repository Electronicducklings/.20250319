#include "kernel.h"
#include<QDebug>
#include"./def/def.h"
#include"def/simulateFunc.h"
#include<QMessageBox>
#include"mainwidget.h"


Kernel::Kernel(QObject *parent) : QObject(parent),
    m_pLogin(new Login),m_pMainWidget(new MainWidget),m_arrDealFun{0}
{
    m_pLogin->show();
    //把Login类型的对象m_pLogin调用signals_login函数当作启动slots_login函数的信号；登录框信号和kernel槽绑定连接
    connect( m_pLogin,&Login::signals_login, this ,&Kernel ::slots_login     );
    //同理
    connect( m_pLogin,&Login::signals_register, this ,&Kernel ::slots_register   );
    //绑定 返回和接收的数据
    connect(this,&Kernel::signals_returnClientData,this,&Kernel::slots_returnClientData);
    //绑定 mainwidget 聊天信息的信号
    connect(m_pMainWidget,&MainWidget::sendMsgAndIdToKernel,this,&Kernel::slots_sendMsgToServer);
    //绑定处理添加好友请求
    connect(m_pMainWidget,&MainWidget::AddFriend,this, &Kernel::slots_ProtAddFriend);
    //绑定接收来自mainwidget的程序退出消息，由Kernel发给服务端，回收资源
    connect(m_pMainWidget, &MainWidget::signal_notifyClose,this,&Kernel::slots_myOfflin);
    //绑定来自Login的程序退出消息,回收资源
    connect(m_pLogin, &Login::signals_delete,this,&Kernel::slots_delete);
    //绑定映射函数
     m_arrDealFun[DEF_PROT_REGISTER_RS- PROT_BASE]=&Kernel::dealProtRegisterRS;//绑定注册回复的
     m_arrDealFun[DEF_PROT_LOGIN_RS  - PROT_BASE] =&Kernel::dealProtLoginRS;//绑定登录回复的
     m_arrDealFun[DEF_PROT_FRIEND_INFO  - PROT_BASE] =&Kernel::dealProtFriendInfo;//绑定处理自己和朋友信息
     m_arrDealFun[DEF_PROT_Chat_INFO_RS  - PROT_BASE] =&Kernel::dealProtChatInfoRs;//绑定服务端回复聊天信息
     m_arrDealFun[DEF_PROT_Chat_INFO_RQ  - PROT_BASE] =&Kernel::dealProtFriendChatInfoRq;//绑定处理朋友的聊天请求
     m_arrDealFun[DEF_PROT_ADD_FRIEND_RS - PROT_BASE] =&Kernel::dealPortAddFriend;//绑定处理朋友的聊天请求
     m_arrDealFun[DEF_PROT_OFFLINE - PROT_BASE] =&Kernel::dealPortFriendOffline;//绑定处理好友下线业务
     m_arrDealFun[DEF_PROT_ADD_FRIEND_RQ - PROT_BASE] =&Kernel::dealPortFriendAddMe;//绑定定时请求添加我为好友
}
//接收登录框登录信息的信号
void Kernel::slots_login(QString tel,QString pass)
{
    qDebug()<<"slots_login"<<"  tel: "<<tel <<"  pass: "<<pass;

    //把登录的数据发给服务端
    PROT_LOGIN_RQ  loginRq;
    strcpy_s(loginRq.tel,30,tel.toStdString().c_str()/*将QString类型数据先转化为string然后转化为const string*/);
    strcpy_s(loginRq.pass,30,pass.toStdString().c_str());
    simulate_login(loginRq);



}

//接收登录框的注册信息的信号
void Kernel::slots_register(QString tel,QString pass,QString nick)
{
     qDebug()<<"slots_register";

     //将注册的数据 打包成数据包
     PROT_register_RQ  registerRq;
     strcpy_s(registerRq.tel,30,tel.toStdString().c_str()/*将QString类型数据先转化为string然后转化为const string*/);
     strcpy_s(registerRq.pass,30,pass.toStdString().c_str());
     strcpy_s(registerRq.nick,30,nick.toStdString().c_str());

     //发送数据包给服务端（模拟）
    simulate_regisert  (  registerRq);


}
//统一接收，所有协议的回复
void Kernel::slots_returnClientData(char* pBuf ,int len,unsigned long from)
{
  qDebug()<<"slots_reutrnClient";
   ProtType pt=*( ProtType*)pBuf;

    qDebug()<<" slots_returnClientData的 prottype(pt)： "<<pt;



   if(pt>= PROT_BASE&&pt<=PROT_BASE+DEAL_FUN_LEN )
   {
       DealFun df=m_arrDealFun[pt-PROT_BASE];

       //校验指针不为空
       if(df)
       {
           (this->*df)(pBuf,len,from);
       }else{
           qDebug()<<"指针为空 指针类型为： "<<df;
       }

   }else{
       qDebug()<<"协议号有错，协议号为： "<<pt;
   }
}
//接收聊天的信息和接收信息朋友的id并将这些发给服务端
void  Kernel::slots_sendMsgToServer(QString msg,int friendid)
{
    qDebug()<<" slots_sendMsgToServer";
    qDebug()<<"msg: "<<msg<<" , "<<friendid;

    PROT_Chat_INFO_RQ chatRq;
    strcpy_s(chatRq.msg,msgLenth,msg.toStdString().c_str());
    chatRq.senderId=m_pMainWidget->getUserId();
    chatRq.receiverId=friendid;

    simulate_chat(chatRq);

}

//处理添加好友请求
 void Kernel::slots_ProtAddFriend(QString nick)
{
      qDebug()<<QString("Kernel收到处理添加%1为好友请求").arg(nick);
      PROT_ADD_FRIEND_RQ addFriendRq;
      addFriendRq.senderId=m_pMainWidget->getUserId();
      strcpy_s( addFriendRq.senderNick  ,30, m_pMainWidget->getMyNick().toStdString().c_str()  );
      strcpy_s( addFriendRq.receiverNick ,30, nick.toStdString().c_str()  );

      //将信息发给服务端（模拟）
      simulate_addFriend( addFriendRq) ;
}

 //定时处理好友下线
 void  Kernel::slots_friOffTimer()
 {
     qDebug()<<"Kernel::slots_friOffTimer";
     PROT_OFFLINE OffFriend;
     OffFriend.friendId=11;
     RETURN_BUF_DATA(   PROT_OFFLINE ,OffFriend);
 }
 //定时改变好友信息
 void  Kernel::slots_FriendChangeInformtion()
 {
      qDebug()<<"Kernel::slots_FriendChangeInformtion";
      PROT_FRIEND_INFO change_Friend_Information;
      change_Friend_Information.userid=20; //这个不要变，变动的话，账号就不是原来那个账号了
      change_Friend_Information.iconid=26;
      strcpy_s(change_Friend_Information.nick,30,"勇往直前");
      strcpy_s(change_Friend_Information.feeling,100,"我真的不想上课鸭呀鸭");
      RETURN_BUF_DATA(  PROT_FRIEND_INFO ,change_Friend_Information);
 }
 //定时请求添加我为好友的信号
 void Kernel::slots_FriendAddMe()
 {
     qDebug()<<"________________Kernel::slots_FriendAddMe------------------";
     PROT_ADD_FRIEND_RQ FriendAddMe;
     FriendAddMe.senderId=17;
     strcpy_s(FriendAddMe.senderNick,30,"老李");
     strcpy_s(FriendAddMe.receiverNick,30,"徐佑恒");

     RETURN_BUF_DATA( PROT_ADD_FRIEND_RQ,FriendAddMe);


 }

 //接收来自mainwidget的程序退出消息，由Kernel发给服务端
 void Kernel::slots_myOfflin()
 {
      qDebug()<<"Kernel::slots_myOfflin";
      PROT_OFFLINE userInfo;
      userInfo.friendId=m_pMainWidget->getUserId();

      //发给服务端
      UerOffline(userInfo);

      //回收客户端资源
      slots_delete();

 }

 //回收客户端资源
 void Kernel::slots_delete()
 {
     qDebug()<<"Kernel::slots_delete";
     if(m_pLogin)
     {
        delete m_pLogin;
        m_pLogin=nullptr;
     }
     if(m_pMainWidget)
     {
         delete m_pMainWidget;
         m_pMainWidget=nullptr;
     }

 }

//-------------------------------------------------------------------------------------------
//处理服务端发的注册信息
void Kernel::dealProtRegisterRS(char*pBuf ,int ,unsigned long )
{
      qDebug()<<"dealProtRegisterRS";
      PROT_register_RS* RegisterRs= ( PROT_register_RS*)pBuf;
      if(RegisterRs->result==Register_Sucess)
      {
          QMessageBox::information(nullptr,"提示","注册成功");
      }else{
          QMessageBox::information(nullptr,"提示","注册失败");
      }


}

//处理服务端发的登录信息
void Kernel::dealProtLoginRS(char*pBuf ,int  ,unsigned long )
{
    qDebug()<<"dealProtLoginRS";

    PROT_LOGIN_RS* LoginRs=(PROT_LOGIN_RS*)pBuf;

    if(LOGIN_Sucess ==LoginRs->result)
    {
        //登录成功
        //隐藏登录界面
        m_pLogin->hide();
        //显示主聊天窗口
        m_pMainWidget->show();
        //存储用户id
        m_pMainWidget->setUerId(  LoginRs->userid);

        //定时让好友下线的定时器
        m_friend_OffLine_Timer.start(3000);//设置定时器时间为三秒
        m_friend_OffLine_Timer.setSingleShot(true);//单次生效
        //绑定定时器处理函数
        connect(&m_friend_OffLine_Timer,SIGNAL(timeout()),this,SLOT(slots_friOffTimer()) );


        //模拟好友改变信息的定时器
        m_friend_Change_information.start(6000);  //设置定时器时间为六秒
        m_friend_Change_information.setSingleShot(true);//单次生效
        //绑定定时器处理函数
        connect(&m_friend_Change_information,SIGNAL(timeout()),this,SLOT(slots_FriendChangeInformtion()) );


        //模拟请求添加我为好友的定时器
        m_FriendAddMe.start(7000);//设置定时器时间为六秒
        m_FriendAddMe.setSingleShot(true);//单次生效
        //绑定定时器处理函数
        connect(&m_FriendAddMe,SIGNAL(timeout()),this,SLOT(slots_FriendAddMe()) );

    }else if(NOEXIT==LoginRs->result)
    {
        QMessageBox::warning(nullptr,"警告","用户不存在");

    }else if(PASSERR  ==LoginRs->result)
    {
        QMessageBox::warning(nullptr,"警告","密码错误");
    }



}

 //处理自己或朋友信息的回复
void Kernel::dealProtFriendInfo(char* pBuf,int ,unsigned long)
{
      qDebug()<<" Kernel::dealProtFriendInfo";
      PROT_FRIEND_INFO* pbuf=(PROT_FRIEND_INFO*)pBuf;

      if(pbuf->userid==m_pMainWidget->getUserId())
      {
        //如果是自己的话
        //根据服务端回复的数据拉取资源
        m_pMainWidget->setMyInfo(pbuf->iconid,pbuf->nick,pbuf->feeling);

      }else{
        //如果是朋友的话
        qDebug()<<" 昵称：  " <<pbuf->nick;
        m_pMainWidget->setFriInfo(pbuf->userid,pbuf->iconid,pbuf->status,pbuf->feeling,pbuf->nick);
      }



}

//处理聊天回复
void Kernel:: dealProtChatInfoRs( char* pBuf,int ,unsigned long)
{
     qDebug()<<"dealProtChatInfoRs";
     PROT_Chat_INFO_RS* chatRs= (PROT_Chat_INFO_RS*)pBuf;
     if(Chat_RESULT_SUCCESS ==chatRs->result)
     {

        m_pMainWidget->findFriend(QString("<font color='gray'>%1</font>").arg("已送达"),chatRs->senderId);
     }else if(Chat_RESULT_FAIL ==chatRs->result){
        m_pMainWidget->findFriend(QString("<font color='gray'>%1</font>").arg("未送达"),chatRs->senderId);
     }

}

//处理朋友发信息的请求
void  Kernel::dealProtFriendChatInfoRq( char*pBuf ,int ,unsigned long)
{
     qDebug()<<"dealProtFriendChatInfoRq";
     PROT_Chat_INFO_RQ* pbuf= (PROT_Chat_INFO_RQ*)pBuf;
     QString s=QString("<p><font size= '5'>%1</font></p>").arg(pbuf->msg);
     m_pMainWidget->findFriend(s ,pbuf->senderId);

}

//处理添加朋友的请求
void Kernel::dealPortAddFriend( char*pBuf ,int ,unsigned long)
{

     PROT_ADD_FRIEND_RS* pbuf= (PROT_ADD_FRIEND_RS*)pBuf;
     qDebug()<<"dealPortAddFriend"<<QString("Kernel收到了%1回复%2添加好友的请求").arg(pbuf->senderNick).arg(pbuf->receiverNick);
     qDebug()<<pbuf->senderNick;

     if("小明"==QString(pbuf->senderNick))
     {
        //加小明 ，在线，且同意
       QMessageBox::information(nullptr,"提示","小明同意了你的好友请求");

     }else if("张三"==QString(pbuf->senderNick)){
        //加张三 ，在线，且拒绝
       QMessageBox::information(nullptr,"提示","张三拒绝了你的好友请求");

     }else if("王五"==QString(pbuf->senderNick)){
         //加王五 ，离线，
       QMessageBox::information(nullptr,"提示","用户不在线");

     }else{
         //用户不存在
       QMessageBox::information(nullptr,"提示","用户不存在");

     }



}

//处理好友下线业务

void Kernel::dealPortFriendOffline( char* pBuf,int ,unsigned long)
{
   PROT_OFFLINE* pbuf=(PROT_OFFLINE*)pBuf;
   qDebug()<<" Kernel::dealPortFriendOffline";

   m_pMainWidget->OfflineFriend(pbuf->friendId);

}

//处理好友改变个人信息
void Kernel:: dealPortFriendChangeInformtion( char* pBuf,int ,unsigned long)
{
     qDebug()<<"Kernel:: dealPortFriendChangeInformtion";
}

//处理定时请求添加我为好友
void Kernel::dealPortFriendAddMe(char*pBuf ,int ,unsigned long)
{
     qDebug()<<" Kernel::dealPortFriendAddMe";
     PROT_ADD_FRIEND_RQ* pbuf= (PROT_ADD_FRIEND_RQ*)pBuf;
     QMessageBox::StandardButton  stdBut=QMessageBox::information(nullptr,"提示",QString("%1请求添加我为好友").arg(pbuf->senderNick),QMessageBox::Yes|QMessageBox::No);

     PROT_ADD_FRIEND_RS addFriRs;
     if(stdBut==QMessageBox::Yes)
     {
         addFriRs.result=ADDFRIEND_AGREE  ;
     }else if(stdBut==QMessageBox::No){
         addFriRs.result=ADDFRIEND_REJECT   ;
     }

     strcpy_s(addFriRs.receiverNick,30,pbuf->senderNick);
     strcpy_s(addFriRs.senderNick,30,pbuf->receiverNick);
     addFriRs.receiverId=pbuf->senderId;
     addFriRs.senderId=m_pMainWidget->getUserId();

     simu_addFriendRs( addFriRs);

}
