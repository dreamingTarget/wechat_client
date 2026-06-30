#include "pagechat.h"
#include "global.h"
#include "ui_pagechat.h"
// #include "chatitembase.h"
// #include "textbubble.h"
// #include "picturebubble.h"

#include <QPainter>
#include <QStyleOption>

PageChat::PageChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageChat)
{
    ui->setupUi(this);
    // ui->pushButton_emo->setState("normal", "hover", "press");
    // ui->pushButton_file->setState("normal", "hover", "press");
    // ui->pushButton_receive->setState("normal", "hover", "press");
    // ui->pushButton_send->setState("normal", "hover", "press");
}

PageChat::~PageChat()
{
    delete ui;
}

// void PageChat::paintEvent(QPaintEvent *event)
// {
//     QStyleOption opt;
//     opt.init(this);
//     QPainter p(this);
//     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//     Q_UNUSED(event);
// }

// void PageChat::on_pushButton_send_clicked()
// {
//     auto pTextEdit = ui->textEdit_chat;
//     ChatRole role = ChatRole::Self;
//     QString userName = QStringLiteral("不信人间有秃头");
//     QString userIcon = ":/picture/RaceAgainstTime.jpg";

//     const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
//     for(int i=0; i<msgList.size(); ++i)
//     {
//         QString type = msgList[i].msgFlag;
//         ChatItemBase *pChatItem = new ChatItemBase(role);
//         pChatItem->setUsername(userName);
//         pChatItem->setUserIcon(userIcon);
//         QWidget *pBubble = nullptr;
//         if(type == "text")
//         {
//             pBubble = new TextBubble(role, msgList[i].content);
//         }
//         else if(type == "image")
//         {
//             pBubble = new PictureBubble(QPixmap(msgList[i].content) , role);
//         }
//         else if(type == "file")
//         {

//         }
//         if(pBubble != nullptr)
//         {
//             pChatItem->setWidget(pBubble);
//             ui->widget_chat_data_list->appendChatItem(pChatItem);
//         }
//     }
// }

