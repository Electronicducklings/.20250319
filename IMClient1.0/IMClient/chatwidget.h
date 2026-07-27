#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    void setMsg(QString msg/*提示消息是否送达*/);

public slots:
    //发送文本输入框的文本  的槽函数
    void slots_send();
     //清空文本输入框的槽函数
    void slots_clear();
signals:
    //向上层 frienditem 发送 聊天内容
   void signals_sendMsg(QString msg);


private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
