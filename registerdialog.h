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

private slots:
    void on_pushButton_get_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> m_handlers;

};

#endif // REGISTERDIALOG_H
