#include "timerbtn.h"

#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget*parent) : QPushButton(parent), m_counter(60) {
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [this](){
        m_counter--;
        if(m_counter <= 0){
            reset();
            return;
        }
        setText(QString::number(m_counter));
    });
}

TimerBtn::~TimerBtn()
{
    reset();
}

void TimerBtn::reset()
{
    m_timer->stop();
    setText("获取");
    m_counter = 60;
    setEnabled(true);
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug("MyButton was release!");
        setEnabled(false);
        setText(QString::number(m_counter));
        m_timer->start();
        emit clicked();
    }
    QPushButton::mouseReleaseEvent(event);
}
