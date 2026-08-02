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
    m_fullMsg = msg;   // 保存完整消息

    // 加载头像（代码不变）
    QPixmap pix(head);
    QSize logicSize = ui->label_icon->size();
    qreal dpr = ui->label_icon->devicePixelRatioF();
    QSize physicalSize = logicSize * dpr;
    pix = pix.scaled(physicalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pix.setDevicePixelRatio(dpr);
    ui->label_icon->setPixmap(pix);
    ui->label_icon->setAlignment(Qt::AlignCenter);

    ui->label_chatname->setText(name);
    updateMsgLabel();  // 调用更新函数
}

void ChatUserWidget::resizeEvent(QResizeEvent *event)
{
    updateMsgLabel();  // 窗口大小变化时重新省略
    QWidget::resizeEvent(event);
}

void ChatUserWidget::updateMsgLabel()
{
    if (m_fullMsg.isEmpty()) {
        ui->label_userchat->setText("");
        return;
    }

    QFontMetrics fm(ui->label_userchat->font());
    // 使用 contentsRect 获取实际可用绘制区域
    int availableWidth = ui->label_userchat->contentsRect().width();
    // 如果标签没有边距，可以用 width() - margin()*2 - indent()
    if (availableWidth <= 0) {
        availableWidth = ui->label_userchat->width() - ui->label_userchat->margin() * 2 - ui->label_userchat->indent();
    }

    // 如果还偏小，打印调试看看实际值
    // qDebug() << "label width:" << ui->label_userchat->width() << "contentsRect:" << ui->label_userchat->contentsRect();

    QString elided = fm.elidedText(m_fullMsg, Qt::ElideRight, availableWidth);
    ui->label_userchat->setText(elided);
}

