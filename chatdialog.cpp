#include "chatdialog.h"
#include "chatuserwidget.h"
#include "conuseritem.h"
#include "loadingdia.h"
#include "qaction.h"
#include "qtimer.h"
#include "tcpmgr.h"
#include "ui_chatdialog.h"
#include "usermanager.h"

#include <QRandomGenerator>

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog), m_mode(ChatUIMode::CHAT_MODE)
    , m_state(ChatUIMode::CHAT_MODE), m_loading(false), m_cur_chat_uid(0)
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

    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_friend_apply);

    //连接认证添加好友信号
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_add_auth_friend, this, &ChatDialog::slot_add_auth_friend);

    //链接自己认证回复信号
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_auth_rsp, this,
            &ChatDialog::slot_auth_rsp);

    //连接searchlist跳转聊天信号
    connect(ui->listWidget_search, &SearchList::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item);

    //连接加载联系人的信号和槽函数
    connect(ui->listWidget_conn_user, &ContactUserList::sig_loading_contact_user, this, &ChatDialog::slot_loading_contact_user);
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
    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserManager::getInstance()->getChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = m_chat_items_added.find(friend_ele->m_uid);
            if(find_iter != m_chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWidget();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->setInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->listWidget_chat_user->addItem(item);
            ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);
            m_chat_items_added.insert(friend_ele->m_uid, item);
        }

        //更新已加载条目
        UserManager::getInstance()->updateChatLoadedCount();
    }

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

void ChatDialog::setSelectChatItem(int uid)
{
    if(ui->listWidget_chat_user->count() <= 0){
        return;
    }

    if(uid == 0){
        ui->listWidget_chat_user->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->listWidget_chat_user->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->listWidget_chat_user->itemWidget(firstItem);
        if(!widget){
            return;
        }

        auto con_item = qobject_cast<ChatUserWidget*>(widget);
        if(!con_item){
            return;
        }

        m_cur_chat_uid = con_item->getUserInfo()->m_uid;

        return;
    }

    auto find_iter = m_chat_items_added.find(uid);
    if(find_iter == m_chat_items_added.end()){
        qDebug() << "uid " <<uid<< " not found, set curent row 0";
        ui->listWidget_chat_user->setCurrentRow(0);
        return;
    }

    ui->listWidget_chat_user->setCurrentItem(find_iter.value());

    m_cur_chat_uid = uid;
}

void ChatDialog::setSelectChatPage(int uid)
{
    // if( ui->chat_user_list->count() <= 0){
    //     return;
    // }

    // if (uid == 0) {
    //     auto item = ui->chat_user_list->item(0);
    //     //转为widget
    //     QWidget* widget = ui->chat_user_list->itemWidget(item);
    //     if (!widget) {
    //         return;
    //     }

    //     auto con_item = qobject_cast<ChatUserWid*>(widget);
    //     if (!con_item) {
    //         return;
    //     }

    //     //设置信息
    //     auto user_info = con_item->GetUserInfo();
    //     ui->chat_page->SetUserInfo(user_info);
    //     return;
    // }

    // auto find_iter = _chat_items_added.find(uid);
    // if(find_iter == _chat_items_added.end()){
    //     return;
    // }

    // //转为widget
    // QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
    // if(!widget){
    //     return;
    // }

    // //判断转化为自定义的widget
    // // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    // ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    // if(!customItem){
    //     qDebug()<< "qobject_cast<ListItemBase*>(widget) is nullptr";
    //     return;
    // }

    // auto itemType = customItem->GetItemType();
    // if(itemType == CHAT_USER_ITEM){
    //     auto con_item = qobject_cast<ChatUserWid*>(customItem);
    //     if(!con_item){
    //         return;
    //     }

    //     //设置信息
    //     auto user_info = con_item->GetUserInfo();
    //     ui->chat_page->SetUserInfo(user_info);

    //     return;
    // }

}

void ChatDialog::loadMoreChatUser()
{
    auto friend_list = UserManager::getInstance()->getChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = m_chat_items_added.find(friend_ele->m_uid);
            if(find_iter != m_chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWidget();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->setInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->listWidget_chat_user->addItem(item);
            ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);
            m_chat_items_added.insert(friend_ele->m_uid, item);
        }

        //更新已加载条目
        UserManager::getInstance()->updateChatLoadedCount();
    }
}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserManager::getInstance()->getConListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto *chat_user_wid = new ConUserItem();
            chat_user_wid->setInfo(friend_ele->m_uid,friend_ele->m_name,
                                   friend_ele->m_icon);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->listWidget_chat_user->addItem(item);
            ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);
        }

        //更新已加载条目
        UserManager::getInstance()->updateContactLoadedCount();
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

void ChatDialog::slot_friend_apply(std::shared_ptr<AddFriendApply> apply)
{
    qDebug() << "receive apply friend slot, applyuid is " << apply->m_from_uid << " name is "
             << apply->m_name << " desc is " << apply->m_desc;

    bool b_already = UserManager::getInstance()->alreadyApply(apply->m_from_uid);
    if (b_already) {
        return;
    }

    UserManager::getInstance()->addApplyList(std::make_shared<ApplyInfo>(apply));
    ui->label_side_contact->showRedPoint(true);
    ui->listWidget_conn_user->showRedPoint(true);
    ui->page_apply->addNewApply(apply);
}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->m_uid
             << " name is " << auth_info->m_name << " nick is " << auth_info->m_nick;

    bool isFriend = UserManager::getInstance()->checkFriendById(auth_info->m_uid);
    if(isFriend){
        return;
    }
    UserManager::getInstance()->addFriend(auth_info);

    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWidget();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->setInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->listWidget_chat_user->insertItem(0, item);
    ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);
    m_chat_items_added.insert(auth_info->m_uid, item);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->m_uid
             << " name is " << auth_rsp->m_name << " nick is " << auth_rsp->m_nick;

    bool isFriend = UserManager::getInstance()->checkFriendById(auth_rsp->m_uid);
    if(isFriend){
        return;
    }
    UserManager::getInstance()->addFriend(auth_rsp);

    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWidget();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->setInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->listWidget_chat_user->insertItem(0, item);
    ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);
    m_chat_items_added.insert(auth_rsp->m_uid, item);
}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item " << endl;
    auto find_iter = m_chat_items_added.find(si->m_uid);
    if(find_iter != m_chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << si->m_uid;
        ui->listWidget_chat_user->scrollToItem(find_iter.value());
        ui->label_side_chat->setSelected(true);
        setSelectChatItem(si->m_uid);
        //更新聊天界面信息
        setSelectChatPage(si->m_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWidget();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->setInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->listWidget_chat_user->insertItem(0, item);
    ui->listWidget_chat_user->setItemWidget(item, chat_user_wid);

    m_chat_items_added.insert(si->m_uid, item);

    ui->label_side_chat->setSelected(true);
    setSelectChatItem(si->m_uid);
    //更新聊天界面信息
    setSelectChatPage(si->m_uid);
    slot_side_chat();
}

void ChatDialog::slot_loading_contact_user()
{
    if(m_loading){
        return;
    }

    m_loading = true;
    LoadingDia *loadingDialog = new LoadingDia(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreChatUser();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    m_loading = false;
}


