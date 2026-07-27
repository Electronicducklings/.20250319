#include "chatwidget.h"
#include "ui_chatwidget.h"
#include<QDebug>
#include<QMessageBox>
#include<QTime>

ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    /*将点击“发送”按钮这个信号与槽函数 ChatWidget::slots_send绑定*/
    connect(ui->pb_send,&QPushButton::clicked,this,&ChatWidget::slots_send);
     /*将点击“清空”按钮这个信号与槽函数 ChatWidget::slots_clear绑定*/
    connect(ui->pb_clear,&QPushButton::clicked,this,& ChatWidget::slots_clear);
}

ChatWidget::~ChatWidget()
{
    qDebug()<<"ChatWidget::~ChatWidget()";
    delete ui;
}

//发送文本输入框的文本  的槽函数
void ChatWidget::slots_send()
{
    qDebug()<<"ChatWidget::slots_send ";


    //获取输入框的聊天内容（纯文本形式）
    QString text=ui->te_msg->toPlainText();

    if(text.isEmpty())
    {
       QMessageBox::information(this,"提示","文本输入框不可以为空");
    }


    //获取输入框的聊天内容（不是纯文本形式，是html形式）
    QString textHtml=ui->te_msg->toHtml();

    text=QString(" <font color='gray'>我[%1]</font>   ").arg(QTime::currentTime().toString() ) +
            "<font size= '5'>"+textHtml+"</front>";

    //将内容复制给聊天框
    ui->tb_record->append(text);
    ui->te_msg->clear();

    //to do:向上层传递聊天内容
    emit  signals_sendMsg( textHtml);

}
 //清空文本输入框的槽函数
void ChatWidget::slots_clear()
{
     qDebug()<<"ChatWidget::slots_clear ";
     ui->te_msg->clear();
}

void ChatWidget::setMsg(QString msg/*提示消息是否送达*/)
{
     qDebug()<<"ChatWidget::setMsg";
     ui->tb_record->append(msg);

}
