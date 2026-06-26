#include "logindialog.h"
#include <QRegularExpression>
#include "global.h"
#include "httpmgr.h"
#include <QJsonObject>
#include "tcpmgr.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
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
    initHttpHandlers();
    connect(HttpMgr::getInstance().get(), &HttpMgr::signal_login_mod_finish, this,
            &LoginDialog::slot_login_mod_finish);

    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    ui->label_pwd_visible->setCursor(Qt::PointingHandCursor);
    ui->label_pwd_visible->setState("unvisible","unvisible_hover","","visible",
                                    "visible_hover","");
    connect(ui->label_pwd_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->label_pwd_visible->getCurState();
        if(state == ClickLbState::Normal){
            ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
        }else{
            ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
        }
    });

    //连接tcp连接请求的信号和槽函数
    connect(this, &LoginDialog::signal_connect_tcp, TcpMgr::getInstance().get(), &TcpMgr::slot_tcp_connect);
    //连接tcp管理者发出的连接成功信号
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_conn_success, this, &LoginDialog::slot_tcp_conn_finished);
    //连接tcp管理者发出的连接失败信号
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_login_failed, this, &LoginDialog::slot_tcp_conn_failed);

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

bool LoginDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->lineEdit_email->text();
    //邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    // 执行正则表达式匹配
    bool match = regex.match(email).hasMatch();
    if (!match) {
        //提示邮箱不正确
        addTipErr(TipErr::TIP_EMAIL_ERR, "邮箱地址不正确");
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;

}

bool LoginDialog::checkPwdValid()
{
    auto pass = ui->lineEdit_pwd->text();
    if (pass.length() < 6 || pass.length() > 15) {
        //提示长度不准确
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regex(R"(^[a-zA-Z0-9!@#$%^&*]{6,15}$)");
    bool match = regex.match(pass).hasMatch();
    if (!match) {
        //提示字符非法
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }

    delTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::addTipErr(TipErr te, QString tips)
{
    m_tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::delTipErr(TipErr te)
{
    m_tip_errs.remove(te);
    if (m_tip_errs.empty()) {
        ui->label_err_tip->clear();
        return;
    }
    showTip(m_tip_errs.first(), false);
}

void LoginDialog::showTip(QString str, bool ok)
{
    ui->label_err_tip->setText(str);
    if (ok) ui->label_err_tip->setProperty("state", "normal");
    else ui->label_err_tip->setProperty("state", "err");
    repolish(ui->label_err_tip);
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->pushButton_login->setEnabled(enabled);
    ui->pushButton_register->setEnabled(enabled);
    ui->label_forget_pwd->setEnabled(enabled);
    return true;
}

void LoginDialog::initHttpHandlers()
{
    //注册获取登录回包逻辑
    m_handlesMap.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            enableBtn(true);
            return;
        }
        auto email = jsonObj["email"].toString();

        //发送信息通知tcpmgr发送长连接
        ServerInfo info;
        info.uid = jsonObj["uid"].toInt();
        info.host = jsonObj["host"].toString();
        info.port = jsonObj["port"].toString();
        info.token = jsonObj["token"].toString();

        m_uid = info.uid;
        m_token = info.token;
        qDebug()<< "email is " << email << " uid is " << info.uid <<" host is "
                 << info.host << " Port is " << info.port << " Token is " << info.token;
        emit signal_connect_tcp(info);
    });
}

void LoginDialog::on_pushButton_login_clicked()
{
    qDebug() << "login btn clicked";
    if (checkEmailValid() == false) return;
    if (checkPwdValid() == false) return;

    enableBtn(false);
    auto email = ui->lineEdit_email->text();
    auto pwd = ui->lineEdit_pwd->text();

    //发送http请求登录
    QJsonObject obj;
    obj["email"] = email;
    QByteArray passwdHashBa = QCryptographicHash::hash(pwd.toUtf8(), QCryptographicHash::Sha256).toHex();
    obj["passwd"] = QJsonValue(QString::fromUtf8(passwdHashBa));
    HttpMgr::getInstance()->postHttpReq(QUrl(gate_url_prefix + "/user_login"),
                                            obj, ReqId::ID_LOGIN_USER, Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"),false);
        return;
    }

    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull()){
        showTip(tr("json解析错误"),false);
        return;
    }

    //json解析错误
    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }

    //调用对应的逻辑,根据id回调。
    m_handlesMap[id](jsonDoc.object());

    return;

}

void LoginDialog::slot_tcp_conn_finished(bool success)
{
    if (success) {
        showTip(tr("聊天服务器连接成功，正在登录......"), true);
        qDebug() << "聊天服务器连接成功，正在登录......";
        QJsonObject obj;
        obj["uid"] = m_uid;
        obj["token"] = m_token;

        QJsonDocument doc(obj);
        QString jsonstr = doc.toJson(QJsonDocument::Indented);
        //发送tcp请求给chatserver
        emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonstr);
    } else {
        showTip(tr("网络异常"), false);
        enableBtn(true);
    }
}

void LoginDialog::slot_tcp_conn_failed(int err)
{
    QString result = QString(tr("登陆失败，err is %1")).arg(err);
    showTip(result, false);
    enableBtn(true);
}

