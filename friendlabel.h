#ifndef FRIENDLABEL_H
#define FRIENDLABEL_H

#include <QFrame>
#include <QWidget>

class QMouseEvent;

namespace Ui {
class FriendLabel;
}

class FriendLabel : public QFrame
{
    Q_OBJECT

public:
    explicit FriendLabel(QWidget *parent = nullptr);
    ~FriendLabel();

    void setText(QString text);
    int width();
    int height();
    QString text();

signals:
    void sig_close(QString str);
    void sig_clicked();

public slots:
    void slot_close();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_text;
    int m_width;
    int m_height;

private:
    Ui::FriendLabel *ui;
};

#endif // FRIENDLABEL_H
