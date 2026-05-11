#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"

#include <QRegularExpression>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);

    ui->label_err_tip->setProperty("state", "normal");
    repolish(ui->label_err_tip);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::showTip(QString str, bool ok)
{
    if (ok) {
        ui->label_err_tip->setProperty("state", "normal");
    } else {
        ui->label_err_tip->setProperty("state", "err");
    }
    ui->label_err_tip->setText(str);
    repolish(ui->label_err_tip);
}

void RegisterDialog::on_pushButton_get_clicked()
{
    auto email = ui->lineEdit_email->text();
    const static QRegularExpression regexp(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regexp.match(email).hasMatch();
    if(match) {
        //发送http验证码

        return;
    } else {
        showTip(tr("邮箱地址不正确"), false);
    }
}

