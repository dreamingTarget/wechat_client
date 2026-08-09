#include "searchlist.h"

#include "adduseritem.h"
#include "customizeedit.h"
#include "findfaildlg.h"
#include "findsuccessdialog.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "tcpmgr.h"
#include "usermanager.h"
#include <QScrollBar>
// #include "customizeedit.h"
// #include "findsuccessdialog.h"
// #include "loadingdia.h"

SearchList::SearchList(QWidget *parent)
: QListWidget(parent), m_send_pending(false), m_find_dlg(nullptr), m_search_edit(nullptr)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);
    //连接点击的信号和槽
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_item_clicked);
    //添加条目
    addTipItem();
    //连接搜索条目
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::closeFindDlg()
{
    if (m_find_dlg) {
        m_find_dlg->hide();
        m_find_dlg = nullptr;
    }
}

void SearchList::setSearchEdit(QWidget *edit)
{
    m_search_edit = edit;
}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}

void SearchList::waitPending(bool pending)
{
    if (pending) {
        // 如果已有 loading，先清理防止重复
        if (m_loadingItem) {
            int index = this->row(m_loadingItem);  // 用 this-> 明确调用成员函数
            if (index != -1) {
                takeItem(index);           // 从列表移除（但不删除）
            }
            delete m_loadingItem;           // 手动删除 item
            m_loadingItem = nullptr;
            if (m_loadingDialog) {
                delete m_loadingDialog;     // 删除对话框
                m_loadingDialog = nullptr;
            }
        }
        // 创建新的 loading
        m_loadingDialog = new LoadingDia;
        m_loadingItem = new QListWidgetItem;
        m_loadingItem->setSizeHint(m_loadingDialog->size());
        addItem(m_loadingItem);
        setItemWidget(m_loadingItem, m_loadingDialog);
        m_send_pending = true;
    } else {
        // 移除并清理
        if (m_loadingItem) {
            int index = this->row(m_loadingItem);
            if (index != -1) {
                takeItem(index);
            }
            delete m_loadingItem;
            m_loadingItem = nullptr;
        }
        if (m_loadingDialog) {
            delete m_loadingDialog;
            m_loadingDialog = nullptr;
        }
        m_send_pending = false;
    }
}

void SearchList::addTipItem()
{
    auto *invalid_item = new QWidget();
    QListWidgetItem *item_tmp = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    this->setItemWidget(item_tmp, invalid_item);
    item_tmp->setFlags(item_tmp->flags() & ~Qt::ItemIsSelectable);


    auto *add_user_item = new AddUserItem();
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); //获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->getItemType();
    if(itemType == ListItemType::INVALID_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

    if(itemType == ListItemType::ADD_USER_TIP_ITEM){
        if (m_send_pending) {
            return;
        }
        if (!m_search_edit) return;
        waitPending(true);
        auto search_edit = dynamic_cast<CustomizeEdit*>(m_search_edit);
        auto uid_str = search_edit->text();
        //此处发送请求给server
        QJsonObject jsonObj;
        jsonObj["uid"] = uid_str;

        QJsonDocument doc(jsonObj);
        QByteArray jsonString = doc.toJson(QJsonDocument::Indented);

        //发送tcp请求给chat server
        emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonString);
        return;

        // //todo ...
        // m_find_dlg = std::make_shared<FindSuccessDialog>(this);
        // auto si = std::make_shared<SearchInfo>(0,"llfc","llfc","hello , my friend!",0);
        // (std::dynamic_pointer_cast<FindSuccessDialog>(m_find_dlg))->setSearchInfo(si);
        // m_find_dlg->show();
        // return;
    }
    //清楚弹出框
    closeFindDlg();

}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);
    if (si == nullptr) {
        m_find_dlg = std::make_shared<FindFailDlg>(this);
    }else{
        //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
        //查找是否已经是好友
        // bool bExist = UserManager::getInstance()->checkFriendById(si->m_uid);
        // if(bExist){
        //     //此处处理已经添加的好友，实现页面跳转
        //     //跳转到聊天界面指定的item中
        //     emit sig_jump_chat_item(si);
        //     return;
        // }
        //此处先处理为添加的好友
        m_find_dlg = std::make_shared<FindSuccessDialog>(this);
        std::dynamic_pointer_cast<FindSuccessDialog>(m_find_dlg)->setSearchInfo(si);

    }
    m_find_dlg->show();
}
