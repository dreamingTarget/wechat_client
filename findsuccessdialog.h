#ifndef FINDSUCCESSDIALOG_H
#define FINDSUCCESSDIALOG_H

#include "userdata.h"
#include <memory>
#include <QDialog>

namespace Ui {
class FindSuccessDialog;
}

class FindSuccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindSuccessDialog(QWidget *parent = nullptr);
    ~FindSuccessDialog();

    void setSearchInfo(std::shared_ptr<SearchInfo> si);

private slots:
    void on_pushButton_add_friend_clicked();

private:
    std::shared_ptr<SearchInfo> m_si;
    QWidget* m_parent;

private:
    Ui::FindSuccessDialog *ui;
};

#endif // FINDSUCCESSDIALOG_H
