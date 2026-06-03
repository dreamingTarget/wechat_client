#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "httpmgr.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
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

    connect(HttpMgr::getInstance().get(), &HttpMgr::signal_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
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

void RegisterDialog::initHttpHandlers()
{
    // 注册模块的处理器
    m_handlers[ReqId::ID_GET_VERIFY_CODE] = [this](const QJsonObject& jsonObj) {
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::SUCCESS) {
            showTip(tr("验证码已成功发送"), true);
            auto email = jsonObj["email"].toString();
            qDebug() << "email is " << email;
        } else {
            showTip(tr("注册失败"), false);
            return;
        }
    };

        // 用户注册模块的处理器
    m_handlers[ReqId::ID_REG_USER] = [this](const QJsonObject& jsonObj) {
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::SUCCESS) {
            showTip(tr("注册成功"), true);
            // emit sig_switch_login();
            auto email = jsonObj["email"].toString();
            qDebug() << "email is " << email;
            qDebug() << "user uid is " << jsonObj["uid"].toString();

        } else {
            showTip(tr("注册失败"), false);
            return;
        }
    };
}

void RegisterDialog::on_pushButton_get_clicked()
{
    auto email = ui->lineEdit_email->text();
    const static QRegularExpression regexp(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regexp.match(email).hasMatch();
    if(match) {
        //发送http验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::getInstance()->postHttpReq(QUrl(gate_url_prefix + "/get_verifycode"), json_obj,
                                            ReqId::ID_GET_VERIFY_CODE, Modules::REGISTERMOD);

        qDebug() << gate_url_prefix;
        return;
    } else {
        showTip(tr("邮箱地址不正确"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull() || !jsonDoc.isObject()) {
        showTip(tr("数据解析错误"), false);
        return;
    }

    m_handlers[id](jsonDoc.object());
}


void RegisterDialog::on_pushButton_OK_clicked()
{
    if(ui->lineEdit_user->text() == ""){
        showTip(tr("用户名不能为空"), false);
        return;
    }
    if(ui->lineEdit_email->text() == ""){
        showTip(tr("邮箱不能为空"), false);
        return;
    }
    if(ui->lineEdit_pwd->text() == ""){
        showTip(tr("密码不能为空"), false);
        return;
    }
    if(ui->lineEdit_confirm->text() == ""){
        showTip(tr("确认密码不能为空"), false);
        return;
    }
    if(ui->lineEdit_pwd->text() != ui->lineEdit_confirm->text()){
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }
    if(ui->lineEdit_verify->text() == ""){
        showTip(tr("验证码不能为空"), false);
        return;
    }
    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->lineEdit_user->text();
    json_obj["email"] = ui->lineEdit_email->text();
    json_obj["passwd"] = ui->lineEdit_pwd->text();
    json_obj["confirm"] = ui->lineEdit_confirm->text();
    json_obj["verifycode"] = ui->lineEdit_verify->text();
    HttpMgr::getInstance()->postHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}

