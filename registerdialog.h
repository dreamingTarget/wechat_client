#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QMap>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private:
    void showTip(QString str, bool ok);
    void initHttpHandlers();

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPwdValid();
    bool checkConfirmValid();
    bool checkVerifyValid();
    void addTipErr(TipErr te, QString tips);
    void delTipErr(TipErr te);
    void changeTipPage();

signals:
    void signal_switch_login();

private slots:
    void on_pushButton_get_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_pushButton_OK_clicked();

    void on_pushButton_ret_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> m_handlers;
    QMap<TipErr, QString> m_tip_errs;

    QTimer* m_timer;
    int m_count;
};

#endif // REGISTERDIALOG_H
