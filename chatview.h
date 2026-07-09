#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

class ChatView : public QWidget
{
    Q_OBJECT
public:
    explicit ChatView(QWidget *parent = nullptr);

    void appendChatItem(QWidget* item);//尾插
    void prependChatItem(QWidget* item);//头插
    void insertChatItem(QWidget* before, QWidget* item);//中间插入

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;
    void paintEvent(QPaintEvent* e) override;

private:
    void initStyleSheet();

signals:

private slots:
    void onVScrollBarMoved(int min, int max);

private:
    QVBoxLayout* m_vbl;
    QScrollArea* m_sa;
    bool m_isAppend;
};

#endif // CHATVIEW_H
