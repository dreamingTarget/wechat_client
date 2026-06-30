#ifndef PAGECHAT_H
#define PAGECHAT_H

#include <QWidget>

namespace Ui {
class PageChat;
}

class PageChat : public QWidget
{
    Q_OBJECT

public:
    explicit PageChat(QWidget *parent = nullptr);
    ~PageChat();

// protected:
//     void paintEvent(QPaintEvent* event) override;

// private slots:
//     void on_pushButton_send_clicked();

private:
    Ui::PageChat *ui;
};

#endif // PAGECHAT_H
