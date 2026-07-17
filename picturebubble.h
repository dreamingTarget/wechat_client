#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H

#include "bubbleframe.h"
#include <QHBoxLayout>
#include <QPixmap>

class QLabel;

class PictureBubble : public BubbleFrame
{
    Q_OBJECT
public:
    PictureBubble(const QPixmap& picture, ChatRole role, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint m_dragStartPos;
    QLabel * lb;
};

#endif // PICTUREBUBBLE_H
