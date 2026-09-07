#include "conuseritem.h"
#include "ui_conuseritem.h"

ConUserItem::ConUserItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ConUserItem)
{
    ui->setupUi(this);
    setItemType(ListItemType::CONTACT_USER_ITEM);
    ui->label_redpoint->setPixmap(QPixmap(":/picture/red_point.png").scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    showRedPoint(false);
    ui->label_redpoint->raise();
}

ConUserItem::~ConUserItem()
{
    delete ui;
}

QSize ConUserItem::sizeHint() const
{
    return QSize(250, 70); // 返回自定义的尺寸
}

void ConUserItem::setInfo(std::shared_ptr<AuthInfo> auth_info)
{
    m_info = std::make_shared<UserInfo>(auth_info);
    // 加载图片
    QPixmap pixmap(m_info->m_icon);

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

    ui->label_username->setText(m_info->m_name);
}

void ConUserItem::setInfo(int uid, QString name, QString icon)
{
    m_info = std::make_shared<UserInfo>(uid,name, icon);

    // 加载图片
    QPixmap pixmap(m_info->m_icon);

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

    ui->label_username->setText(m_info->m_name);
}

void ConUserItem::setInfo(std::shared_ptr<AuthRsp> auth_rsp){
    m_info = std::make_shared<UserInfo>(auth_rsp);

    // 加载图片
    QPixmap pixmap(m_info->m_icon);

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

    ui->label_username->setText(m_info->m_name);
}

void ConUserItem::showRedPoint(bool show)
{
    if(show){
        ui->label_redpoint->show();
    }else{
        ui->label_redpoint->hide();
    }

}
