#include "chatuserlist.h"

ChatUserList::ChatUserList(QWidget *parent) : QListWidget(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //安装事件过滤器
    installEventFilter(this);
}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
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
            //滚动到底部，加载新的联系人
            qDebug() << "load more chat user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_chat_user();
        }
        return true;//停止事件传递
    }
    return QListWidget::eventFilter(watched, event);
}
