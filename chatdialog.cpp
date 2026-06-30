#include "chatdialog.h"
#include "chatuserwidget.h"
#include "qaction.h"
#include "ui_chatdialog.h"

#include <QRandomGenerator>

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog), m_mode(ChatUIMode::CHAT_MODE)
    , m_state(ChatUIMode::CHAT_MODE), m_loading(false)
{
    ui->setupUi(this);
    ui->pushButton_add->setState("normal", "hover", "press");

    QAction* clearAct = new QAction(ui->lineEdit_search);
    clearAct->setIcon(QIcon(":/picture/close_transparent.png"));
    ui->lineEdit_search->addAction(clearAct, QLineEdit::TrailingPosition);
    connect(ui->lineEdit_search, &CustomizeEdit::textChanged, this, [=](const QString& text){
        if (!text.isEmpty()) {
            clearAct->setIcon(QIcon(":/picture/close_search.png"));
        } else {
            clearAct->setIcon(QIcon(":/picture/close_transparent.png"));
        }
    });

    connect(clearAct, &QAction::triggered, this, [=](){
        ui->lineEdit_search->clear();
        clearAct->setIcon(QIcon(":/picture/close_transparent.png"));
        ui->lineEdit_search->clearFocus();
        showSearch(false);
    });
    showSearch(false);
    addChatUserList();
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::showSearch(bool search)
{
    if (search) {
        ui->listWidget_chat_user->hide();
        ui->listWidget_conn_user->hide();
        ui->listWidget_search->show();
        m_mode = ChatUIMode::SEARCH_MODE;
    } else if (m_state == ChatUIMode::CHAT_MODE) {
        ui->listWidget_chat_user->show();
        ui->listWidget_conn_user->hide();
        ui->listWidget_search->hide();
        m_mode = ChatUIMode::CHAT_MODE;
    } else if (m_state == ChatUIMode::CONTACT_MODE) {
        ui->listWidget_chat_user->hide();
        ui->listWidget_conn_user->show();
        ui->listWidget_search->hide();
        m_mode = ChatUIMode::CONTACT_MODE;
    }
}


void ChatDialog::addChatUserList()
{
    for (int i = 0; i < 13; i++) {
        int random = QRandomGenerator::global()->bounded(100);
        int str = random % strs.size();
        int head = random % heads.size();
        int name = random % names.size();

        auto* chat_user_wid = new ChatUserWidget;
        chat_user_wid->setInfo(names[name], heads[head], strs[str]);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->listWidget_chat_user->addItem(item);
        ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);
    }
}

