#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_register, &QPushButton::clicked, this, [this]() {
        emit signal_switch_register();
    });

    ui->label_forget_pwd->setState("normal","hover","","selected","selected_hover","");
    connect(ui->label_forget_pwd, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);

    initHeadImage();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug("slot forget pwd");
    emit signal_switch_reset();
}

void LoginDialog::initHeadImage()
{
    // //加载图片
    // QPixmap pix(":/picture/RaceAgainstTime.jpg");
    // //设置图片自动缩放
    // qDebug() << pix.size() << ui->label_picture->size();
    // pix = pix.scaled(ui->label_picture->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // qDebug() << pix.size();
    // //创建一个和原始图片相同大小的pixmap，用于绘制圆角图片
    // QPixmap round_pix(pix.size());
    // round_pix.fill(Qt::transparent);//用透明色填充

    // QPainter p(&round_pix);
    // p.setRenderHint(QPainter::Antialiasing);//设置抗锯齿，使圆角更平滑
    // p.setRenderHint(QPainter::SmoothPixmapTransform);

    // //使用painterpath设置圆角
    // QPainterPath pp;
    // pp.addRoundedRect(0, 0, pix.width(), pix.height(), 10, 10);
    // p.setClipPath(pp);

    // //将原图片绘制到round_pix上
    // p.drawPixmap(0, 0, pix);
    // //设置绘制好的圆角图片到label上
    // ui->label_picture->setPixmap(round_pix);
    // ui->label_picture->setAlignment(Qt::AlignCenter);

    // 你的Label是200*200正方形，先定义固定尺寸常量（方便修改）
    const int LABEL_WIDTH = 200;
    const int LABEL_HEIGHT = 200;

    // 加载图片
    QPixmap pix(":/picture/RaceAgainstTime.jpg");
    // 打印尺寸用于调试（新增：打印原始图片尺寸，对比裁剪后尺寸）
    qDebug() << "原始图片尺寸：" << pix.size() << " Label尺寸：" << QSize(LABEL_WIDTH, LABEL_HEIGHT);

    // ========== 新增核心步骤：裁剪图片为1:1正方形（保持美观，避免拉伸） ==========
    QPixmap croppedPix;
    if (pix.width() != pix.height()) { // 非正方形图片才需要裁剪
        // 1. 取图片宽/高的较小值作为裁剪后正方形的边长
        int cropEdge = qMin(pix.width(), pix.height());
        // 2. 计算裁剪起始坐标（从图片中心裁剪，保留核心内容，最美观）
        int cropX = (pix.width() - cropEdge) / 2;
        int cropY = (pix.height() - cropEdge) / 2;
        // 3. 裁剪出中心区域的1:1正方形图片
        croppedPix = pix.copy(cropX, cropY, cropEdge, cropEdge);
    } else { // 已经是正方形，直接复用
        croppedPix = pix;
    }

    // ========== 修改缩放步骤：将裁剪后的正方形图片缩放到200*200（填满Label，无拉伸） ==========
    pix = croppedPix.scaled(
        LABEL_WIDTH, LABEL_HEIGHT, // 直接指定Label的200*200尺寸
        Qt::KeepAspectRatio,        // 此时图片已是1:1，缩放无拉伸，仅适配尺寸
        Qt::SmoothTransformation    // 平滑缩放，保证清晰度（美观关键）
        );

    // 创建一个和缩放后图片（200*200）相同大小的pixmap，用于绘制圆角图片
    QPixmap round_pix(pix.size());
    round_pix.fill(Qt::transparent);//用透明色填充（避免圆角外有白色边框）

    QPainter p(&round_pix);
    p.setRenderHint(QPainter::Antialiasing);//设置抗锯齿，使圆角更平滑（美观关键）
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    // 使用painterpath设置圆角（固定半径20，适配200*200正方形，美观协调）
    QPainterPath pp;
    // 圆角半径设为20（可调整：15-25之间最适配200*200正方形，过大过小圆角都不协调）
    pp.addRoundedRect(0, 0, pix.width(), pix.height(), 20, 20);
    p.setClipPath(pp);

    // 将缩放后的200*200图片绘制到round_pix上（填满Label，无拉伸）
    p.drawPixmap(0, 0, pix);

    // 设置绘制好的圆角图片到label上（填满200*200Label，美观无违和）
    ui->label_image->setPixmap(round_pix);
    // 补充：确保Label不额外拉伸图片，填满自身
    ui->label_image->setScaledContents(true);
    ui->label_image->setFixedSize(LABEL_WIDTH, LABEL_HEIGHT); // 锁定Label为200*200正方形

}

