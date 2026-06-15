#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMap>
#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    void initHeadImage();
    bool checkEmailValid();
    bool checkPwdValid();
    void addTipErr(TipErr te, QString tips);
    void delTipErr(TipErr te);
    void showTip(QString str, bool ok);
    bool enableBtn(bool enabled);
    void initHttpHandlers();

signals:
    void signal_switch_register();
    void signal_switch_reset();
    void signal_connect_tcp(ServerInfo info);

private slots:
    void slot_forget_pwd();

    void on_pushButton_login_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);

    void slot_tcp_conn_finished(bool success);
    void slot_tcp_conn_failed(int err);

private:
    Ui::LoginDialog *ui;
    QMap<TipErr, QString> m_tip_errs;
    QMap<ReqId, std::function<void(const QJsonObject&)>> m_handlesMap;
    int m_uid;
    QString m_token;
};

#endif // LOGINDIALOG_H
