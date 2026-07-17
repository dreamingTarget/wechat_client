#include "chatitembase.h"

ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent)
    : QWidget{parent}, m_role(role)
{
    m_name = new QLabel;
    m_name->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_name->setFont(font);
    m_name->setFixedHeight(20);

    m_icon = new QLabel;
    m_icon->setScaledContents(true);
    m_icon->setFixedSize(42, 42);

    m_bubble = new QWidget;

    QGridLayout* gl = new QGridLayout;
    gl->setMargin(3);
    gl->setVerticalSpacing(3);
    gl->setHorizontalSpacing(3);

    QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    if (m_role == ChatRole::Self) {
        m_name->setContentsMargins(0, 0, 8, 0);
        m_name->setAlignment(Qt::AlignRight);
        gl->addWidget(m_name, 0, 1, 1, 1);
        gl->addWidget(m_icon, 0, 2, 2, 1, Qt::AlignTop);
        gl->addItem(spacer, 1, 0, 1, 1);
        gl->addWidget(m_bubble, 1, 1, 1, 1);
        gl->setColumnStretch(0, 2);
        gl->setColumnStretch(1, 3);
    } else {
        m_name->setContentsMargins(8, 0, 0, 0);
        m_name->setAlignment(Qt::AlignLeft);
        gl->addWidget(m_icon, 0, 0, 2, 1, Qt::AlignTop);
        gl->addWidget(m_name, 0, 1, 1, 1);
        gl->addWidget(m_bubble, 1, 1, 1, 1);
        gl->addItem(spacer, 2, 2, 1, 1);
        gl->setColumnStretch(1, 3);
        gl->setColumnStretch(2, 2);
    }
    setLayout(gl);
}

void ChatItemBase::setUsername(const QString &name)
{
    m_name->setText(name);
}

void ChatItemBase::setUserIcon(const QString &icon)
{
    m_icon->setPixmap(QPixmap(icon));
}

void ChatItemBase::setWidget(QWidget *w)
{
    QGridLayout* gl = qobject_cast<QGridLayout*>(layout());
    gl->replaceWidget(m_bubble, w);
    delete m_bubble;
    m_bubble = w;
    gl->setAlignment(w, Qt::AlignRight | Qt::AlignVCenter);
}
