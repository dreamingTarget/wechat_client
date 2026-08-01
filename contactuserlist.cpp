#include "contactuserlist.h"
#include "conuseritem.h"
#include "global.h"
#include "grouptipitem.h"
#include "listitembase.h"
#include "QRandomGenerator"
// #include "tcpmgr.h"

ContactUserList::ContactUserList(QWidget *parent) : QListWidget(parent) {
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    installEventFilter(this);

    //模拟从数据库或者后端传输过来的数据,进行列表加载
    addContactUserList();
    //连接点击的信号和槽
    connect(this, &QListWidget::itemClicked, this, &ContactUserList::slot_item_clicked);
    //    //链接对端同意认证后通知的信号
    //    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend,this,
    //            &ContactUserList::slot_add_auth_firend);

    //    //链接自己点击同意认证后界面刷新
    //    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp,this,
    //            &ContactUserList::slot_auth_rsp);
}

void ContactUserList::showRedPoint(bool bshow)
{
    m_add_friend_item->showRedPoint(bshow);
}

bool ContactUserList::eventFilter(QObject *watched, QEvent *event)
{
    //检查事件是否是鼠标悬浮进入或离开
    if (watched == this) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    //检查事件是否是鼠标滚轮事件
    if (watched == this && event->type() == QEvent::Wheel) {
        QWheelEvent* e = static_cast<QWheelEvent*>(event);
        int numDegress = e->angleDelta().y() / 8;
        int numSteps = numDegress / 15;//计算滚动步数
        //设置滚动幅度
        verticalScrollBar()->setValue(verticalScrollBar()->value() - numSteps);

        //检查是否滚动到底部
        QScrollBar* scrollBar = verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int curValue = scrollBar->value();
        //int pageSize = 10; //每页加载的联系人数量

        // qDebug() << maxScrollValue << ", " << curValue;

        if (maxScrollValue - curValue <= 0) {
            // 滚动到底部，加载新的联系人
            qDebug()<<"load more contact user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_contact_user();
        }
        return true;//停止事件传递
    }
    return QListWidget::eventFilter(watched, event);
}

void ContactUserList::addContactUserList()
{
    auto * groupTip = new GroupTipItem();
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(groupTip->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, groupTip);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    m_add_friend_item = new ConUserItem();
    m_add_friend_item->setObjectName("new_friend_item");
    m_add_friend_item->setInfo(0,tr("新的朋友"),":/picture/add_friend.png");
    m_add_friend_item->setItemType(ListItemType::APPLY_FRIEND_ITEM);

    QListWidgetItem *add_item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    add_item->setSizeHint(m_add_friend_item->sizeHint());
    this->addItem(add_item);
    this->setItemWidget(add_item, m_add_friend_item);

    //默认设置新的朋友申请条目被选中
    this->setCurrentItem(add_item);

    auto * groupCon = new GroupTipItem();
    groupCon->setGroupTip(tr("联系人"));
    m_groupitem = new QListWidgetItem;
    m_groupitem->setSizeHint(groupCon->sizeHint());
    this->addItem(m_groupitem);
    this->setItemWidget(m_groupitem, groupCon);
    m_groupitem->setFlags(m_groupitem->flags() & ~Qt::ItemIsSelectable);


    // 创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *con_user_wid = new ConUserItem();
        con_user_wid->setInfo(str_i,names[name_i], heads[head_i]);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(con_user_wid->sizeHint());
        this->addItem(item);
        this->setItemWidget(item, con_user_wid);
    }
}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); // 获取自定义widget对象
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
    if(itemType == ListItemType::INVALID_ITEM
        || itemType == ListItemType::GROUP_TIP_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

    if(itemType == ListItemType::APPLY_FRIEND_ITEM){

        // 创建对话框，提示用户
        qDebug()<< "apply friend item clicked ";
        //跳转到好友申请界面
        emit sig_switch_apply_friend_page();
        return;
    }

    if(itemType == ListItemType::CONTACT_USER_ITEM){
        // 创建对话框，提示用户
        qDebug()<< "contact user item clicked ";
        //跳转到好友申请界面
        emit sig_switch_friend_info_page();
        return;
    }
}
