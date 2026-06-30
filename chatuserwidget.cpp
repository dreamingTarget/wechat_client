#include "chatuserwidget.h"
#include "ui_chatuserwidget.h"

ChatUserWidget::ChatUserWidget(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ChatUserWidget)
{
    ui->setupUi(this);
    setItemType(CHAT_USER_ITEM);
}

ChatUserWidget::~ChatUserWidget()
{
    delete ui;
}

void ChatUserWidget::setInfo(QString name, QString head, QString msg)
{
    m_name = name;
    m_head = head;
    m_msg = msg;

    //加载图片
    QPixmap pix(head);

    QSize logicSize = ui->label_icon->size();
    qreal dpr = ui->label_icon->devicePixelRatioF();
    QSize physicalSize = logicSize * dpr;
    pix = pix.scaled(physicalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pix.setDevicePixelRatio(dpr);

    ui->label_icon->setPixmap(pix);
    ui->label_icon->setAlignment(Qt::AlignCenter);
    ui->label_chatname->setText(name);
    ui->label_userchat->setText(msg);
}
