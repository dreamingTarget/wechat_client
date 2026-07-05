#include "listitembase.h"
#include "qpainter.h"
#include "qstyleoption.h"

ListItemBase::ListItemBase(QWidget *parent)
    : QWidget{parent}
{}

void ListItemBase::setItemType(ListItemType type)
{
    m_type = type;
}

ListItemType ListItemBase::getItemType()
{
    return m_type;
}

void ListItemBase::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
