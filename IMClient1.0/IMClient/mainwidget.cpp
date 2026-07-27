#include "mainwidget.h"
#include "ui_mainwidget.h"
#include<QDebug>
#include"frienditem.h"
#include<QMessageBox>
#include<QInputDialog>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),m_pMenu(new QMenu),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    connect(ui->pb_menu,&QPushButton::clicked,this, &MainWidget:: clickMenu);//绑定点击菜单按钮
    //创建菜单后，添加菜单选项，并且把选项的地址接住，方便以后调用
    m_pAddfriend= m_pMenu->addAction("添加好友");
    m_pSysSet=m_pMenu->addAction("系统设置");

    //void QMenu::triggered(QAtion*)  /*点击菜单的选项的信号,qt里自带的信号，不是自己创的*/
    //点击菜单的选项时，会返回选项地址（triggered(QAction*)函数的返回值），将选项地址交给slots_dealMenu(QAction * action)
    connect(  m_pMenu/*菜单指针*/  ,SIGNAL(triggered(QAction*))/*点击菜单的选项的信号*/,this,SLOT(slots_dealMenu(QAction * ))  );

}


MainWidget::~MainWidget()
{
    qDebug()<<"MainWidget::~MainWidget()";
    delete ui;
    //释放菜单
    if(m_pMenu)
    {
        delete m_pMenu;
        m_pMenu=nullptr;
    }
    //释放好友项
    for(pair<int , FriendItem*> v :m_pFriendMap)
    {
        delete v.second;

    }

}

 void MainWidget::setMyInfo(int iconid,QString nick,QString feeling)
 {
     //设置昵称
     ui->l_nick->setText(nick);
     //设置签名
     ui->le_sign->setText(feeling);
     //工程中的图片路径
     QString imgPath=QString(":/tx/%1.png").arg(iconid);
     //设置按钮图标
     ui->pb_img->setIcon(QIcon(imgPath));

     //保存自己的信息
      m_iconid=iconid;
      m_nick=nick;
      m_feeling=feeling;

 }

 //设置朋友信息
 void MainWidget::setFriInfo(int userid,int iconid,int status,QString feeling ,QString nick)
 {
     //判断是否已经是好友，如果是，修改朋友信息
    if( m_pFriendMap.count(userid))
    {
        m_pFriendMap[userid]->showFriItem( userid, iconid, status, feeling , nick);
    }else{

         FriendItem* pFriItem=new FriendItem;

         //保存朋友id
         pFriItem->setFriendId(userid);

         //拉取并显示朋友信息
        pFriItem->showFriItem(userid,iconid,status, feeling , nick);

         //创建列表
         QListWidgetItem* pWidgetItem=new QListWidgetItem;
         //添加列表
         ui->lw_friendlist->addItem(pWidgetItem);
         //将  FriendItem 添加到 QListWidgetItem 上面
         ui->lw_friendlist->setItemWidget(pWidgetItem, pFriItem);

         //将  QListWidgetItem 调整到和 FriendItem 高度一致
         //获取大小
         QSize s=pFriItem->size();
         //设置大小
         pWidgetItem->setSizeHint(s);

         /*将  FriendItem::sendMsgAndId 与 MainWidget::sendMsgAndIdToKernel 这两个信号绑定，
          *   以此将  文本信息和朋友id 从frienditem通过mainwidget直接传递给 kernel
        */
        connect(pFriItem,&FriendItem::sendMsgAndId,this,&MainWidget::sendMsgAndIdToKernel);

        //存储朋友信息
        m_pFriendMap[userid]=pFriItem;

      }

 }

 //找到朋友
 void MainWidget:: findFriend( QString msg/*提示消息是否送达*/,int friendId)
 {
     //如果朋友存在
     if( m_pFriendMap.count(friendId))
     {

         m_pFriendMap[friendId]->setChatMsg( msg/*提示消息是否送达*/);

     }
 }
 //点击菜单
 void MainWidget:: clickMenu()
 {
     qDebug()<<" MainWidget:: clickMenu";
     //获取基于屏幕左上角点的光标的位置
     QPoint po =QCursor ::pos();
     //获取菜单的大小
     QSize s=m_pMenu->sizeHint();
     po.setY( po.y()-s.height());

     //在指定的点显示菜单
     m_pMenu->exec(po);

 }
 //点击菜单中的添加好友
 void MainWidget:: slots_dealMenu(QAction * action)
 {
      qDebug()<<" MainWidget:: slots_dealMenu";
      //点击了添加好友选项
      if(m_pAddfriend==action)
      {
           qDebug()<<" 你成功点击了添加好友选项";
           QString text=QInputDialog::getText(this,"添加好友","请输入好友昵称").trimmed();
           if(text.isEmpty())
           {
               QMessageBox::information(this,"提示","昵称不能为空");
               return ;
           }


           //判断是否已经是你的好友
           for( pair<int , FriendItem*> v: m_pFriendMap   )
           {
               if(text==v.second->getFriendNick())
               {
                    QMessageBox::information(this,"提示",text+"已经是你的好友");
                    return ;

               }
           }

           //判断是否为自己
           if(text== m_nick)
           {
               QMessageBox::information(this,"提示","不能添加自己为好友");
               return ;
           }
            //向Kernel发送添加好友的请求
            emit  AddFriend( text);

      }else if(m_pSysSet==action){
           //点击了系统设置选项
           qDebug()<<" 你成功点击了添加系统设置选项";
           QMessageBox::information(this,"提示","该功能暂未开发，敬请期待");
      }

 }

 //让好友下线
 void MainWidget:: OfflineFriend(int FriendId)
 {
     //如果朋友存在
     if( m_pFriendMap.count(FriendId))
     {

         m_pFriendMap[FriendId]->setFriendOffline();
     }
 }

 //重写父类虚函数,将程序退出消息发给Kernel，由Kernel发给服务端
void MainWidget::closeEvent(QCloseEvent* event)
{
    qDebug()<<"MainWidget::closeEvent";
    //将程序退出消息的信号发给Kernel
    emit signal_notifyClose();
}
