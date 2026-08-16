#include "chatdialog.h"
#include "chatuserwidget.h"
#include "loadingdia.h"
#include "qaction.h"
#include "qtimer.h"
#include "ui_chatdialog.h"
#include "usermanager.h"

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
    connect(ui->listWidget_chat_user, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    addChatUserList();

    QString pix = UserManager::getInstance()->icon();
    QPixmap pixmap(pix);

    QSize logicSize = ui->label_side_head->size();
    qreal dpr = ui->label_side_head->devicePixelRatioF();
    QSize physicalSize = logicSize * dpr;
    pixmap = pixmap.scaled(physicalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap.setDevicePixelRatio(dpr);
    ui->label_side_head->setAlignment(Qt::AlignCenter);
    ui->label_side_head->setPixmap(pixmap); // 将图片设置到QLabel上
    ui->label_side_head->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小

    ui->label_side_chat->setState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    // ui->label_side_chat->setProperty("state","pressed");
    // repolish(ui->label_side_chat);
    // update();
    ui->label_side_chat->setSelected(true);

    ui->label_side_contact->setState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    addLabGroup(ui->label_side_chat);
    addLabGroup(ui->label_side_contact);

    connect(ui->label_side_chat, &StateWidget::clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->label_side_contact, &StateWidget::clicked, this, &ChatDialog::slot_side_contact);
    connect(ui->lineEdit_search, &CustomizeEdit::textChanged, this, &ChatDialog::slot_text_changed);

    this->installEventFilter(this);

    ui->listWidget_search->setSearchEdit(ui->lineEdit_search);

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

void ChatDialog::addLabGroup(StateWidget *lab)
{
    m_lab_list.push_back(lab);
}

void ChatDialog::slot_loading_chat_user()
{
    if (m_loading) return;
    m_loading = true;
    LoadingDia* loadDia = new LoadingDia;
    std::shared_ptr<QListWidgetItem> item = std::make_shared<QListWidgetItem>();
    item->setSizeHint(loadDia->size());
    ui->listWidget_chat_user->addItem(item.get());
    ui->listWidget_chat_user->setItemWidget(item.get(), loadDia);

    QTimer::singleShot(500, this, [=](){
        ui->listWidget_chat_user->takeItem(ui->listWidget_chat_user->row(item.get()));
        addChatUserList();
        ui->listWidget_chat_user->scrollToBottom();
        m_loading = false;
    });
}

void ChatDialog::clearLabState(StateWidget *lab)
{
    for (const auto& it : m_lab_list) {
        if (it == lab) continue;
        it->clearState();
    }
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    // 实现点击位置的判断和处理逻辑
    // 先判断是否处于搜索模式，如果不处于搜索模式则直接返回
    if(m_mode != ChatUIMode::SEARCH_MODE){
        qDebug() << "m_mode: " << m_mode;
        return;
    }

    // 将鼠标点击位置转换为搜索列表坐标系中的位置
    QPoint posInSearchList = ui->listWidget_search->mapFromGlobal(event->globalPos());
    // 判断点击位置是否在聊天列表的范围内
    if (!ui->listWidget_search->rect().contains(posInSearchList)) {
        // 如果不在聊天列表内，清空输入框
        ui->lineEdit_search->clear();
        showSearch(false);
    }
}

void ChatDialog::slot_side_chat()
{
    qDebug()<< "receive side chat clicked";
    clearLabState(ui->label_side_chat);
    ui->stackedWidget->setCurrentWidget(ui->page_chat);
    m_state = ChatUIMode::CHAT_MODE;
    showSearch(false);
}

void ChatDialog::slot_side_contact()
{
    qDebug()<< "receive side chat clicked";
    clearLabState(ui->label_side_contact);
    ui->stackedWidget->setCurrentWidget(ui->page_apply);
    m_state = ChatUIMode::CONTACT_MODE;
    showSearch(false);
}

void ChatDialog::slot_text_changed(const QString &str)
{
    if (!str.isEmpty()) {
        showSearch(true);
    }
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched, event);
}


