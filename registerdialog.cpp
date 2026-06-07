#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "httpmgr.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog), m_count(5)
{
    ui->setupUi(this);
    
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);

    ui->label_err_tip->setProperty("state", "normal");
    repolish(ui->label_err_tip);

    connect(HttpMgr::getInstance().get(), &HttpMgr::signal_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();

    ui->label_err_tip->clear();
    connect(ui->lineEdit_user, &QLineEdit::editingFinished, this, [this](){
        checkUserValid();
    });
    connect(ui->lineEdit_email, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->lineEdit_pwd, &QLineEdit::editingFinished, this, [this](){
        checkPwdValid();
    });
    connect(ui->lineEdit_confirm, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->lineEdit_verify, &QLineEdit::editingFinished, this, [this](){
        checkVerifyValid();
    });
    ui->label_pwd_visible->setCursor(Qt::PointingHandCursor);
    ui->label_confirm_visible->setCursor(Qt::PointingHandCursor);
    ui->label_pwd_visible->setState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");
    ui->label_confirm_visible->setState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");

    //连接点击事件
    connect(ui->label_pwd_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->label_pwd_visible->getCurState();
        if(state == ClickLbState::Normal){
            ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
        }else{
            ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });
    connect(ui->label_confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->label_confirm_visible->getCurState();
        if(state == ClickLbState::Normal){
            ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);
        }else{
            ui->lineEdit_confirm->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=](){
        if (m_count == 0) {
            m_timer->stop();
            emit signal_switch_login();
            return;
        }
        m_count--;
        auto str = QString("注册成功，%1秒后返回登录").arg(m_count);
        ui->label_tip->setText(str);
    });
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
            changeTipPage();
        } else {
            showTip(tr("注册失败"), false);
            return;
        }
    };
}

bool RegisterDialog::checkUserValid()
{
    if(ui->lineEdit_user->text() == ""){
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkPwdValid()
{
    auto pass = ui->lineEdit_pwd->text();
    auto confirm = ui->lineEdit_confirm->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    delTipErr(TipErr::TIP_PWD_ERR);
        // 检查密码和确认密码是否一致
    if(pass != confirm){
        addTipErr(TipErr::TIP_PWD_CONFIRM, tr("密码和确认密码不一致"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_CONFIRM);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->lineEdit_email->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->lineEdit_pwd->text();
    auto confirm = ui->lineEdit_confirm->text();
    if(confirm.length() < 6 || confirm.length()>15){
        //提示长度不准确
        addTipErr(TipErr::TIP_CONFIRM_ERR, tr("确认密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(confirm).hasMatch();
    if(!match){
        //提示字符非法
        addTipErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;;
    }
    delTipErr(TipErr::TIP_CONFIRM_ERR);
    // 检查密码和确认密码是否一致
    if(pass != confirm){
        addTipErr(TipErr::TIP_PWD_CONFIRM, tr("确认密码和密码不一致"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_CONFIRM);
    return true;

}

bool RegisterDialog::checkVerifyValid()
{
    auto pass = ui->lineEdit_verify->text();
    if(pass.isEmpty()){
        addTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    delTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

void RegisterDialog::addTipErr(TipErr te, QString tips)
{
    m_tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::delTipErr(TipErr te)
{
    m_tip_errs.remove(te);
    if(m_tip_errs.empty()){
        ui->label_err_tip->clear();
        return;
    }
    showTip(m_tip_errs.first(), false);
}

void RegisterDialog::changeTipPage()
{
    m_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    m_timer->start(1000);
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
    QString passwd = QCryptographicHash::hash(
        ui->lineEdit_pwd->text().toUtf8(),
        QCryptographicHash::Sha256
        ).toHex();
    json_obj["passwd"] = passwd;
    QString confirm = QCryptographicHash::hash(
        ui->lineEdit_pwd->text().toUtf8(),
        QCryptographicHash::Sha256
        ).toHex();
    json_obj["confirm"] = confirm;
    json_obj["verifycode"] = ui->lineEdit_verify->text();
    HttpMgr::getInstance()->postHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}


void RegisterDialog::on_pushButton_ret_clicked()
{
    m_timer->stop();
    emit signal_switch_login();
}


void RegisterDialog::on_pushButton_cancel_clicked()
{
    emit signal_switch_login();
}

