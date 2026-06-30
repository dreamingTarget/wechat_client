#include "listitembase.h"

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
