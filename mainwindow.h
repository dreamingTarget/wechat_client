#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatdialog.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"

/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      主窗口
 *
 * @author     熊嘉宇
 * @date       2026/05/08
 * @history
 *****************************************************************************/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_switch_register();
    void slot_switch_login();
    void slot_switch_reset();
    void slot_reset_switch_login();
    void slot_switch_chat();

private:
    Ui::MainWindow *ui;

    LoginDialog* m_loginDialog;
    RegisterDialog* m_registerDialog;
    ResetDialog* m_resetDialog;
    ChatDialog* m_chatDialog;

};
#endif // MAINWINDOW_H
