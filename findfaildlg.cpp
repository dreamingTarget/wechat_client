#include "findfaildlg.h"
#include "ui_findfaildlg.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>

FindFailDlg::FindFailDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindFailDlg)
{
    ui->setupUi(this);
    // 设置对话框标题
    setWindowTitle("添加");
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("FindFailDlg");
    // ui->btn_fail_sure->setState("normal","hover","press");
    // 获取当前应用程序的路径
    this->setModal(true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(200, 200, 200, 80));  // 浅灰色，透明度80
    shadow->setOffset(0, 2);
    this->setGraphicsEffect(shadow);
}

FindFailDlg::~FindFailDlg()
{
    qDebug()<< "Find FailDlg destruct";
    delete ui;
}



void FindFailDlg::on_btn_fail_sure_clicked()
{
    this->hide();
}
