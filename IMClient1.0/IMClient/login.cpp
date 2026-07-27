#include "login.h"
#include "ui_login.h"
#include<QMessageBox>
#include<QDebug>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    //注册
    connect(       ui->pb_register,SIGNAL( clicked() ),this,   SLOT( slots_register())      );
    //注册清空
     connect(       ui->pb_register_clear,SIGNAL( clicked() ),this,   SLOT( slots_registerClear())      );
     //登录
     connect(   ui->pb_login,SIGNAL(clicked() ),this,  SLOT(slots_login()) );
     //登录清空
     connect(   ui->pb_loginClear,SIGNAL(clicked()),this,  SLOT( slots_loginClear()  )          );
}

Login::~Login()
{
    qDebug()<<"Login::~Login()";

    delete ui;
}
//注册
void Login::slots_register()
{
     //获取所有注册信息
    QString nick=ui->le_nick->text();
    if(nick.isEmpty())
    {
        QMessageBox::information(this/*父窗口*/,"提示","昵称不能为空");
        return;
    }

    //获取电话号码
   QString tel= ui->le_register_tel->text();
   //审核长度
   if(tel.size()!=11)
   {
       QMessageBox::information(this/*父窗口*/,"提示","号码必须11位");
       return ;
   }
   //遍历是否为数字
   for(QChar v :tel)
   {
       if(v<'0'||v>'9')
       {
           QMessageBox::information(this/*父窗口*/,"提示","号码必须数字");
           return;
       }
   }

   //校验密码
   QString pass=ui->le_register_pass->text();
   QString passagain=ui->le_passagain->text();
    if(pass.size()<6||passagain.size()<6)
   {
          QMessageBox::information(this/*父窗口*/,"提示","密码必须不小于6位");
          return ;
    }

    if(pass!=passagain)
    {
        QMessageBox::information(this/*父窗口*/,"提示","两次密码输入不一致，请重新输入");
        return ;
    }

     // to do :注册的信息发给服务端（先把数据发给kernel，kernel发给服务端simulateFunc）
    emit signals_register(tel, pass, nick);

}
//注册清空
void Login:: slots_registerClear()
{
  ui->le_nick->clear();
  ui->le_register_pass->clear();
  ui->le_passagain->clear();
  ui->le_register_tel->clear();

}

//登录
void Login::slots_login()
{
    //获取电话号码
   QString tel= ui->le_tel->text();
   //审核长度
   if(tel.size()!=11)
   {
       QMessageBox::information(this/*父窗口*/,"提示","号码必须11位");
       return ;
   }
   //遍历是否为数字
   for(QChar v :tel)
   {
       if(v<'0'||v>'9')
       {
           QMessageBox::information(this/*父窗口*/,"提示","号码必须数字");
           return;
       }
   }
   //校验密码
   QString pass=ui->le_pass->text();

    if(pass.size()<6)
   {
          QMessageBox::information(this/*父窗口*/,"提示","密码必须不小于6位");
          return ;
    }

   // to do :登录的信息发给服务端（先把数据发给kernel，kernel发给服务端simulateFunc）
      emit signals_login(tel, pass);
}
//登录清空
void Login::slots_loginClear()
{
       ui->le_pass->clear();
       ui->le_tel->clear();
}

//重写父类虚函数,如果还没有登录时，鼠标🖱️点X，程序退出，将程序退出消息发给Kernel，由Kernel发给服务端
void Login::closeEvent(QCloseEvent* event)
{
    qDebug()<<"Login::closeEvent(QCloseEvent* event)";

    //回收客户端资源的信号（还未登录）
    emit signals_delete();
}


