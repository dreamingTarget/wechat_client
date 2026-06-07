#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include "global.h"
#include <QLabel>
#include <QObject>

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget* parent = nullptr);
    void setState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");
    ClickLbState getCurState();

protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

signals:
    void clicked(void);

private:
    QString m_normal;
    QString m_normal_hover;
    QString m_normal_press;
    QString m_selected;
    QString m_selected_hover;
    QString m_selected_press;
    ClickLbState m_curstate;

};

#endif // CLICKEDLABEL_H
