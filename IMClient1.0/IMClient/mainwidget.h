#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <map>
#include"frienditem.h"
#include"./def/def.h"
#include<QMenu>
using namespace std;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    //用户ID的接口
    int getUserId(){return m_userid;}
    //设置用户id
    void setUerId(int userid){m_userid=userid;}
    //设置用户信息
    void setMyInfo(int iconid,QString nick,QString feeling);
    //设置朋友信息
    void setFriInfo(int userid,int iconid,int status,QString feeling ,QString nick);
    //找到朋友
    void findFriend( QString msg/*提示消息是否送达*/,int friendId);
    //让好友下线
    void OfflineFriend(int FriendId);
    //获取自己的昵称
    QString getMyNick(){return m_nick;}
public:
    //重写父类虚函数,如果已经登录，鼠标🖱️点X，程序退出，将程序退出消息发给Kernel，由Kernel发给服务端
    virtual void closeEvent(QCloseEvent* event);
signals:
    //接收来自frienditem的信号，并向kernel发送信号，传递聊天框的文本信息和接收信息的朋友id
    void  sendMsgAndIdToKernel(QString msg,int friendid);
    //向Kernel发送添加好友的请求
    void AddFriend(QString nick);
    //将程序退出消息的信号发给Kernel
    void signal_notifyClose();

public slots:
    //点击菜单
    void clickMenu();
    //点击菜单中的添加好友
    void slots_dealMenu(QAction * action);


    private:
    Ui::MainWidget *ui;
    //自己的信息
    int m_userid;
    int m_iconid;
    QString m_nick;
    QString m_feeling;


    QMenu* m_pMenu;        //菜单
    QAction* m_pAddfriend; //添加好友
    QAction* m_pSysSet;    //系统设置

    //键值： 朋友的id，实值：FriendItem
    map<int , FriendItem*> m_pFriendMap;


};

#endif // MAINWIDGET_H
