#include "chatview.h"
#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QScrollBar>
#include <QStyleOption>
#include <QTimer>

ChatView::ChatView(QWidget *parent)
    : QWidget{parent}, m_isAppend(false)
{
    QVBoxLayout* mainVbl = new QVBoxLayout;
    setLayout(mainVbl);
    mainVbl->setMargin(0);

    m_sa = new QScrollArea;
    m_sa->setObjectName("chat_area");
    mainVbl->addWidget(m_sa);

    QWidget* w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);

    QVBoxLayout* Vbl = new QVBoxLayout;
    Vbl->addWidget(new QWidget(), 100000);
    w->setLayout(Vbl);
    m_sa->setWidget(w);

    m_sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar* sb = m_sa->verticalScrollBar();
    connect(sb, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);

    //把垂直scrollbar放到上边，而不是原来的并排
    QHBoxLayout* hbl = new QHBoxLayout;
    hbl->addWidget(sb, 0, Qt::AlignRight);
    hbl->setMargin(0);
    m_sa->setLayout(hbl);
    sb->setHidden(true);

    m_sa->setWidgetResizable(true);
    m_sa->installEventFilter(this);
    initStyleSheet();
}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout* vbl = qobject_cast<QVBoxLayout*>(m_sa->widget()->layout());
    vbl->insertWidget(vbl->count() - 1, item);
    m_isAppend = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

bool ChatView::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::Enter && obj == m_sa) {
        //maximum() = 内容总高度 - 当前可见区域高度
        m_sa->verticalScrollBar()->setHidden(m_sa->verticalScrollBar()->maximum() == 0);
    } else if (e->type() == QEvent::Leave && obj == m_sa) {
        m_sa->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(obj, e);
}

void ChatView::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatView::initStyleSheet()
{

}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if (m_isAppend) {//添加item可能调用多次
        QScrollBar* sb = m_sa->verticalScrollBar();
        sb->setSliderPosition(sb->maximum());
        //500ms可能调用多次
        QTimer::singleShot(500, this, [=](){
            m_isAppend = false;
        });
    }
}
