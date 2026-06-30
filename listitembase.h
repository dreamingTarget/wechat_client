#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include "global.h"
#include <QWidget>
#include <QObject>

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);

    void setItemType(ListItemType type);
    ListItemType getItemType();

signals:

private:
    ListItemType m_type;

};

#endif // LISTITEMBASE_H
