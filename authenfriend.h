#ifndef AUTHENFRIEND_H
#define AUTHENFRIEND_H

#include <QDialog>
#include <QMap>
#include "clickedlabel.h"
#include "friendlabel.h"
#include "userdata.h"

namespace Ui {
class AuthenFriend;
}

class AuthenFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenFriend(QWidget *parent = nullptr);
    ~AuthenFriend();

    void initTipLbs();
    bool eventFilter(QObject *obj, QEvent *event);
    void setApplyInfo(std::shared_ptr<ApplyInfo> apply_info);

protected:
    //拦截回车键，避免回车直接关闭对话框
    void keyPressEvent(QKeyEvent *event) override;

public:
    void setSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    ClickedLabel* createTipLabel(const QString& text);
    QSize measureTipSize(const QString& text) const;
    void relayoutTipLabels(int maxLines = -1);
    void relayoutFriendLabels();
    void addLabel(QString name);
    void addTipLabelToList(const QString& text, bool selected = true);
    void clearLabelInput();
    QString normalizedLabelText(const QString& text) const;
    void syncTipLabelState(const QString& text, bool selected);
    void updateInputTip(const QString& text);
    void refreshLabelArea();
    void ensureInputVisible();

public slots:
    //显示更多label标签
    void slot_show_more_label();
    //输入label按下回车触发将标签加入展示栏
    void slot_label_enter();
    //点击关闭，移除展示栏好友便签
    void slot_remove_friend_label(QString);
    //通过点击tip实现增加和减少好友便签
    void slot_change_friend_label_by_tip(QString, ClickLbState);
    //输入框文本变化显示不同提示
    void slot_label_text_change(const QString& text);
    //输入框输入完成
    void slot_label_edit_finished();
    //输入标签显示提示框，点击提示框内容后添加好友便签
    void slot_add_firend_label_by_click_tip(QString text);
    //处理确认回调
    void slot_apply_sure();
    //处理取消回调
    void slot_apply_cancel();

private:
    //已经创建好的标签
    QMap<QString, ClickedLabel*> m_add_labels;
    std::vector<QString> m_add_label_keys;
    //用来在输入框显示添加新好友的标签
    QMap<QString, FriendLabel*> m_friend_labels;
    std::vector<QString> m_friend_label_keys;
    std::vector<QString> m_tip_data;
    bool m_tip_expanded = false;
    QString m_live_label_text;
    //只放好友标签的画布，与输入框一起由 widget_grid 的垂直布局管理
    QWidget* m_tag_canvas = nullptr;
    std::shared_ptr<SearchInfo> m_si;
    std::shared_ptr<ApplyInfo> m_apply_info;

private:
    Ui::AuthenFriend *ui;

};

#endif // AUTHENFRIEND_H
