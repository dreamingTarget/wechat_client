#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
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

private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString& str);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::ChatDialog *ui;

    ChatUIMode m_mode;
    ChatUIMode m_state;
    bool m_loading;
    QList<StateWidget*> m_lab_list;
};

#endif // CHATDIALOG_H
