#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

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

private slots:
    void on_pushButton_get_clicked();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
