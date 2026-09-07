#ifndef CHATUSERWIDGET_H
#define CHATUSERWIDGET_H

#include "listitembase.h"
#include "userdata.h"
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
    void setInfo(std::shared_ptr<UserInfo> user_info);
    std::shared_ptr<UserInfo> getUserInfo();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateMsgLabel();  // 更新消息标签的显示

private:
    QString m_name;
    QString m_head;
    QString m_msg;
    QString m_fullMsg;      // 存储完整消息，用于计算省略
    std::shared_ptr<UserInfo> m_user_info;

private:
    Ui::ChatUserWidget *ui;
};

#endif // CHATUSERWIDGET_H
