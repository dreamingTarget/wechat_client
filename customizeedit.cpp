#include "customizeedit.h"
#include <QAction>

CustomizeEdit::CustomizeEdit(QWidget *parent) : QLineEdit(parent) {
    setMaxLength(30);
    connect(this, &CustomizeEdit::textChanged, this, &CustomizeEdit::limitTextLength);

    QAction* searchAction = new QAction(this);
    searchAction->setIcon(QIcon(":/picture/search.png"));
    addAction(searchAction, QLineEdit::LeadingPosition);
    setPlaceholderText(QStringLiteral("搜索"));

}

void CustomizeEdit::setMaxLength(int maxlen)
{
    m_max_len = maxlen;
}

void CustomizeEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit sig_focus_out();
}

void CustomizeEdit::limitTextLength(QString text)
{
    if (m_max_len <= 0) return;
    QByteArray byte = text.toUtf8();
    if (byte.size() > m_max_len) {
        byte = byte.left(m_max_len);
        setText(QString::fromUtf8(byte));
    }
}
