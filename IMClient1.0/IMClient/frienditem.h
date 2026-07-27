#ifndef FRIENDITEM_H
#define FRIENDITEM_H
#include"chatwidget.h"
#include <QWidget>

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendItem(QWidget *parent = nullptr);
    ~FriendItem();
public:
    //拉取并设置朋友信息
    void showFriItem(int userid,int iconid,int status,QString feeling,QString nick);
    //获取朋友id
    int getFriendId(){return m_friendId;}
    //设置朋友的id
    int setFriendId(int friendid){m_friendId=friendid;}
    //获取朋友的昵称
    QString getFriendNick(){return m_nick;}
    //设置消息送达情况
    void setChatMsg(QString msg/*提示消息是否送达*/);
    //设置朋友的在线状态
    void setFriendStatus();
    //设置朋友下线
    void setFriendOffline();
public slots:
    //槽函数：显示聊天框
    void showChatWidget();
    //接收chatwidget聊天内容，并向上层 mainwidget 发送聊天内容和 朋友id
    void slots_sendMsgAndId(QString msg);
signals:
    //向mainwidget发送信号，以此传递聊天框的文本信息和接收信息的朋友id
    void sendMsgAndId(QString msg,int friendid);


private:
    Ui::FriendItem *ui;
     //朋友的信息
    int m_friendId;
    int m_iconid;
    int m_status;
    QString m_feeling;
    QString m_nick;
    ChatWidget *m_pChatWidget;

};

#endif // FRIENDITEM_H
