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
    void inputMethodEvent(QInputMethodEvent* event) override;

private:
    void limitTextLength(QString text);

signals:
    void sig_focus_out(bool flag = false);
    void sig_live_text_changed(const QString& text);

private:
    int m_max_len;

};

#endif // CUSTOMIZEEDIT_H
