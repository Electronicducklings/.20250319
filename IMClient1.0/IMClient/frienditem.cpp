#include "frienditem.h"
#include "ui_frienditem.h"
#include<QDebug>
#include<QBitmap>
#include"def/def.h"
#include<QTime>


FriendItem::FriendItem(QWidget *parent) :
    QWidget(parent),m_pChatWidget(new ChatWidget),
    ui(new Ui::FriendItem)
{
    ui->setupUi(this);
    connect(ui->pb_img,&QPushButton::clicked,this,&FriendItem::showChatWidget);/*将“点击头像”这个信号与槽函数
                                                                                showChatWidget绑定*/
     connect(m_pChatWidget,&ChatWidget::signals_sendMsg,this, &FriendItem::slots_sendMsgAndId);
}

FriendItem::~FriendItem()
{
    qDebug()<<"FriendItem::~FriendItem";
    delete ui;
    if(m_pChatWidget)
    {
        delete  m_pChatWidget;
        m_pChatWidget=nullptr;

    }

}

 //拉取并设置朋友信息
 void FriendItem::showFriItem(int userid,int iconid,int status,QString feeling,QString nick)
 {
     qDebug()<<"FriendItem::showFriItem";
     qDebug()<<" 昵称：  " <<nick;
     qDebug()<<"  头像序号：  "<<iconid;
     //离线
     if(OFFLINE==status)
     {
       qDebug()<<"进头像循环了";
     //工程中的图片路径
     QString imgPath=QString(":/tx/%1.png").arg(iconid);
     //将图片变成灰白
     QBitmap bit(imgPath);
     //设置按钮图标
     ui->pb_img->setIcon(bit);

     }else if(ONLINE==status){
         //在线
         //工程中的图片路径
         QString imgPath=QString(":/tx/%1.png").arg(iconid);
         //设置按钮图标
         ui->pb_img->setIcon(QIcon(imgPath));

      }

     //拉取朋友的昵称
     ui->l_nick_2->setText(nick);
     //拉取朋友的签名
     ui->l_sign->setText(feeling);

     //保存朋友信息
      m_friendId=userid;
      m_iconid =iconid;
      m_status =status;
      m_feeling=feeling;
      m_nick=nick;


 }

 //显示聊天框
 void FriendItem::showChatWidget()
 {
     qDebug()<<" FriendItem::showChatWidget";
     //设置聊天框的标题
    QString s= QString ("与%1的聊天框").arg(m_nick);
     m_pChatWidget->setWindowTitle(s);
     //显示聊天框
     m_pChatWidget->show();

 }

//接收chatwidget聊天内容，并向上层 mainwidget 发送聊天内容和 朋友id
void FriendItem::slots_sendMsgAndId(QString msg)
{
       qDebug()<<"FriendItem::slots_sendMsgAndId    ";
       qDebug()<<"msg: "<<msg;

       emit  sendMsgAndId(msg,m_friendId);
}
 //设置消息送达情况
 void FriendItem::setChatMsg( QString msg/*提示消息是否送达*/)
 {
      qDebug()<<"FriendItem::setMsg   "<<msg;

      QString  s=QString("<font color='gray'>%1[%2]: </font>").arg(m_nick).arg(QTime::currentTime().toString());
      s+=msg;


     m_pChatWidget->setMsg(s);

 }

 //设置朋友下线
 void FriendItem::setFriendOffline()
{
    qDebug()<<"FriendItem::setFriendOffline ";

     //让头像变灰
    qDebug()<<"进头像循环了";
    //工程中的图片路径
    QString imgPath=QString(":/tx/%1.png").arg(m_iconid);
    //将图片变成灰白
    QBitmap bit(imgPath);
    //设置按钮图标
    ui->pb_img->setIcon(bit);


    //修改朋友状态（下线）
    m_status=OFFLINE;
}
