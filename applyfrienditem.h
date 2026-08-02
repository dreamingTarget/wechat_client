#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"
#include <memory>

namespace Ui {
class ApplyFriendItem;
}

class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ApplyFriendItem(QWidget *parent = nullptr);
    ~ApplyFriendItem();
    void setInfo(std::shared_ptr<ApplyInfo> apply_info);
    void showAddBtn(bool bshow);
    QSize sizeHint() const override {
        return QSize(250, 80); // 返回自定义的尺寸
    }
    int getUid();

    void updateMsgLabel();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void sig_auth_friend(std::shared_ptr<ApplyInfo> apply_info);

private:
    Ui::ApplyFriendItem *ui;
    std::shared_ptr<ApplyInfo> m_apply_info;
    bool m_added;
    QString m_fullMsg;      // 存储完整消息，用于计算省略

};

#endif // APPLYFRIENDITEM_H
