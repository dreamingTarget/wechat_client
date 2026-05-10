#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_register, &QPushButton::clicked, this, [this]() {
        emit signal_switch_register();
    });
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
