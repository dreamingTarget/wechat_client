#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H

#include <QObject>
#include <QPushButton>

class ClickedBtn : public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget *parent = nullptr);
    ~ClickedBtn();

    void setState(QString normal, QString hover, QString press);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString m_normal;
    QString m_hover;
    QString m_press;

};

#endif // CLICKEDBTN_H
