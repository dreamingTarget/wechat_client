#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H

#include <QLineEdit>
#include <QObject>

class CustomizeEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomizeEdit(QWidget *parent = nullptr);

    void setMaxLength(int maxlen);

protected:
    void focusOutEvent(QFocusEvent* event) override;

private:
    void limitTextLength(QString text);

signals:
    void sig_focus_out(bool flag = false);

private:
    int m_max_len;

};

#endif // CUSTOMIZEEDIT_H
