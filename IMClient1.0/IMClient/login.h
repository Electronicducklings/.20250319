#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
public:
    //重写父类虚函数,如果还没有登录时，鼠标🖱️点X，程序退出，将程序退出消息发给Kernel，由Kernel发给服务端
    virtual void closeEvent(QCloseEvent* event);
public slots:
    //注册
    void slots_register();
    // 注册的清空
    void slots_registerClear();
    //登录
    void slots_login();
    //登录清空
    void slots_loginClear();

signals:
    //发送给kernel注册和登录的信息
    void signals_login(QString tel,QString pass );
    void signals_register(QString tel,QString pass,QString nick);

    //回收客户端资源的信号（还未登录）
    void signals_delete();

private:
    Ui::Login *ui;
};
#endif // LOGIN_H
