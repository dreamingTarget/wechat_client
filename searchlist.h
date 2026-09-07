#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>
#include <QDialog>
#include <memory>
#include "userdata.h"
#include "loadingdia.h"

class SearchList: public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget *parent = nullptr);
    void closeFindDlg();
    void setSearchEdit(QWidget* edit);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void waitPending(bool pending = true);
    void addTipItem();

signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);

private slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);

private:
    bool m_send_pending;
    std::shared_ptr<QDialog> m_find_dlg;
    QWidget* m_search_edit;
    LoadingDia* m_loadingDialog;
    QListWidgetItem* m_loadingItem = nullptr;

};
#endif // SEARCHLIST_H
