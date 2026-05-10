#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"

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

private:
    Ui::MainWindow *ui;

    LoginDialog* m_loginDialog;
    RegisterDialog* m_registerDialog;

};
#endif // MAINWINDOW_H
