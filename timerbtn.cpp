#include "timerbtn.h"

#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget*parent) : QPushButton(parent), m_counter(60) {
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [this](){
        m_counter--;
        if(m_counter <= 0){
            m_timer->stop();
            m_counter = 60;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        setText(QString::number(m_counter));
    });
}

TimerBtn::~TimerBtn()
{
    m_timer->stop();
    delete m_timer;
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
