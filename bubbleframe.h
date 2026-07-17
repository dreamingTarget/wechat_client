#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

#include <QFrame>
#include "global.h"
#include <QHBoxLayout>

class BubbleFrame : public QFrame
{
    Q_OBJECT
public:
    BubbleFrame(ChatRole role, QWidget* parent = nullptr);

    void setMargin(int margin);
    void setWidget(QWidget* w);

protected:
    void paintEvent(QPaintEvent* e) override;

private:
    QHBoxLayout* m_hbl;
    ChatRole m_role;
    int m_margin;
    const int width_triangle = 8;
};

#endif // BUBBLEFRAME_H
