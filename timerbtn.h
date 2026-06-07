#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QObject>
#include <QPushButton>
#include <QTimer>

class TimerBtn : public QPushButton
{
    Q_OBJECT
public:
    TimerBtn(QWidget*parent = nullptr);
    ~TimerBtn();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QTimer* m_timer;
    int m_counter;
};

#endif // TIMERBTN_H
