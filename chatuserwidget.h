#ifndef CHATUSERWIDGET_H
#define CHATUSERWIDGET_H

#include "listitembase.h"
#include <QWidget>

namespace Ui {
class ChatUserWidget;
}

class ChatUserWidget : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWidget(QWidget *parent = nullptr);
    ~ChatUserWidget();

    QSize sizeHint() const override {
        return QSize(250, 70);
    }
    void setInfo(QString name, QString head, QString msg);

private:
    QString m_name;
    QString m_head;
    QString m_msg;

private:
    Ui::ChatUserWidget *ui;
};

#endif // CHATUSERWIDGET_H
