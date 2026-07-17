#include "bubbleframe.h"

#include <QPainter>

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent)
    : QFrame(parent), m_role(role), m_margin(3) {

    m_hbl = new QHBoxLayout;
    if (m_role == ChatRole::Self) {
        m_hbl->setContentsMargins(m_margin, m_margin, m_margin + width_triangle, m_margin);
    } else {
        m_hbl->setContentsMargins(m_margin + width_triangle, m_margin, m_margin, m_margin);
    }
    setLayout(m_hbl);
}

void BubbleFrame::setMargin(int margin)
{
    Q_UNUSED(margin);
}

void BubbleFrame::setWidget(QWidget *w)
{
    if (m_hbl->count() > 0) {
        return;
    } else {
        m_hbl->addWidget(w);
    }
}

void BubbleFrame::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    if (m_role == ChatRole::Other) {
        //画气泡
        QColor bk_color(Qt::white);
        p.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(width_triangle, 0, width() - width_triangle, height());
        p.drawRoundedRect(bk_rect, 5, 5);
        //画小三角
        QPointF pfs[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10 + width_triangle + 2),
            QPointF(bk_rect.x() - width_triangle, 10 + width_triangle - width_triangle / 2)
        };
        p.drawPolygon(pfs, 3);
    } else {
        QColor bk_color(158, 234, 106);
        p.setBrush(QBrush(bk_color));
        //画气泡
        QRect bk_rect = QRect(0, 0, width() - width_triangle, height());
        p.drawRoundedRect(bk_rect, 5, 5);
        //画小三角
        QPointF pfs[3] = {
            QPointF(bk_rect.x() + bk_rect.width(), 12),
            QPointF(bk_rect.x() + bk_rect.width(), 12 + width_triangle + 2),
            QPointF(bk_rect.x() + bk_rect.width() + width_triangle, 10 + width_triangle - width_triangle / 2)
        };
        p.drawPolygon(pfs, 3);

    }

    return QFrame::paintEvent(e);
}
