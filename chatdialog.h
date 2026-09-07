#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "applyfriend.h"
#include "global.h"
#include "statewidget.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private:
    void showSearch(bool search = false);
    void addChatUserList();
    void addLabGroup(StateWidget* lab);
    void clearLabState(StateWidget* lab);

    void handleGlobalMousePress(QMouseEvent *event);
    void setSelectChatItem(int uid = 0);
    void setSelectChatPage(int uid = 0);
    void loadMoreChatUser();
    void loadMoreConUser();

private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString& str);
    bool eventFilter(QObject *watched, QEvent *event);

    void slot_friend_apply(std::shared_ptr<AddFriendApply> apply);

    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);

    void slot_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void slot_loading_contact_user();

private:
    Ui::ChatDialog *ui;

    ChatUIMode m_mode;
    ChatUIMode m_state;
    bool m_loading;
    QList<StateWidget*> m_lab_list;
    QMap<int, QListWidgetItem*> m_chat_items_added;

    int m_cur_chat_uid;
};

#endif // CHATDIALOG_H
