#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent) :
    ListItemBase(parent), ui(new Ui::ApplyFriendItem),
    m_added(false)
{
    ui->setupUi(this);
    setItemType(ListItemType::APPLY_FRIEND_ITEM);
    ui->btn_add->setState("normal","hover", "press");
    ui->btn_add->hide();
    connect(ui->btn_add, &ClickedBtn::clicked,  [this](){
        emit this->sig_auth_friend(m_apply_info);
    });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::setInfo(std::shared_ptr<ApplyInfo> apply_info)
{
    m_apply_info = apply_info;

    m_fullMsg = m_apply_info->m_desc;   // 保存完整消息
    // 加载图片
    QPixmap pixmap(m_apply_info->m_icon);

    // 设置图片自动缩放
    QSize logicSize = ui->label_icon->size();
    qreal dpr = ui->label_icon->devicePixelRatioF();
    QSize physicalSize = logicSize * dpr;
    pixmap = pixmap.scaled(physicalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap.setDevicePixelRatio(dpr);

    ui->label_icon->setPixmap(pixmap);
    ui->label_icon->setAlignment(Qt::AlignCenter);

    // ui->label_icon->setPixmap(pixmap.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_icon->setScaledContents(true);

    ui->label_username->setText(m_apply_info->m_name);
    // ui->label_user_chat->setText(m_apply_info->m_desc);

    updateMsgLabel();
}

void ApplyFriendItem::showAddBtn(bool bshow)
{
    if (bshow) {
        ui->btn_add->show();
        ui->label_already_add->hide();
        m_added = false;
    }
    else {
        ui->btn_add->hide();
        ui->label_already_add->show();
        m_added = true;
    }
}

int ApplyFriendItem::getUid() {
    return m_apply_info->m_uid;
}

void ApplyFriendItem::updateMsgLabel()
{
    if (m_fullMsg.isEmpty()) {
        ui->label_user_chat->setText("");
        return;
    }

    QFontMetrics fm(ui->label_user_chat->font());
    // 使用 contentsRect 获取实际可用绘制区域
    int availableWidth = ui->label_user_chat->contentsRect().width();
    // 如果标签没有边距，可以用 width() - margin()*2 - indent()
    if (availableWidth <= 0) {
        availableWidth = ui->label_user_chat->width() - ui->label_user_chat->margin() * 2 - ui->label_user_chat->indent();
    }

    // 如果还偏小，打印调试看看实际值
    // qDebug() << "label width:" << ui->label_userchat->width() << "contentsRect:" << ui->label_userchat->contentsRect();

    QString elided = fm.elidedText(m_fullMsg, Qt::ElideRight, availableWidth);
    ui->label_user_chat->setText(elided);
}

void ApplyFriendItem::resizeEvent(QResizeEvent *event)
{
    updateMsgLabel();  // 窗口大小变化时重新省略
    QWidget::resizeEvent(event);
}


