#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "global.h"

class BubbleFrame;

class ChatItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);
    void setUsername(const QString& name);
    void setUserIcon(const QString& icon);
    void setWidget(QWidget* w);

signals:

private:
    ChatRole m_role;
    QLabel* m_name;
    QLabel* m_icon;
    QWidget* m_bubble;

};

#endif // CHATITEMBASE_H
