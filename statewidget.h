#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>
#include "global.h"
#include <QLabel>

class StateWidget : public QLabel
{
    Q_OBJECT
public:
    StateWidget(QWidget *parent = nullptr);

    void setState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");

    ClickLbState getCurState();
    void clearState();

    void setSelected(bool bselected);
    void addRedPoint();
    void showRedPoint(bool show=true);

protected:
    // void paintEvent(QPaintEvent* event) override;

    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
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
    QLabel * m_red_point;

};

#endif // STATEWIDGET_H
