#include "mainwindow.h"
#include "tcpmgr.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_loginDialog = new LoginDialog(this);
    setCentralWidget(m_loginDialog);
    // m_loginDialog->show();
    m_loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(m_loginDialog, &LoginDialog::signal_switch_register, this, &MainWindow::slot_switch_register);

    connect(m_loginDialog, &LoginDialog::signal_switch_reset, this, &MainWindow::slot_switch_reset);
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_switch_chatdlg, this, &MainWindow::slot_switch_chat);
    emit TcpMgr::getInstance()->sig_switch_chatdlg();
}

MainWindow::~MainWindow()
{
    delete ui;

    // if (m_loginDialog)
    // {
    //     delete m_loginDialog;
    //     m_loginDialog = nullptr;
    // }

    // if (m_registerDialog)
    // {
    //     delete m_registerDialog;
    //     m_registerDialog = nullptr;
    // }
}

void MainWindow::slot_switch_register()
{
    m_registerDialog = new RegisterDialog(this);
    m_registerDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    // m_loginDialog->hide();
    setCentralWidget(m_registerDialog);
    // m_registerDialog->show();
    connect(m_registerDialog, &RegisterDialog::signal_switch_login, this, &MainWindow::slot_switch_login);
}

void MainWindow::slot_switch_login()
{
    m_loginDialog = new LoginDialog(this);
    setCentralWidget(m_loginDialog);
    m_loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    connect(m_loginDialog, &LoginDialog::signal_switch_register, this, &MainWindow::slot_switch_register);
    connect(m_loginDialog, &LoginDialog::signal_switch_reset, this, &MainWindow::slot_switch_reset);

}

void MainWindow::slot_switch_reset()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    m_resetDialog = new ResetDialog(this);
    m_resetDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_resetDialog);
    // m_loginDialog->hide();
    // m_resetDialog->show();
    //注册返回登录信号和槽函数
    connect(m_resetDialog, &ResetDialog::signal_switch_login, this, &MainWindow::slot_reset_switch_login);
}

void MainWindow::slot_reset_switch_login()
{
    m_loginDialog = new LoginDialog(this);
    setCentralWidget(m_loginDialog);
    m_loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    connect(m_loginDialog, &LoginDialog::signal_switch_register, this, &MainWindow::slot_switch_register);
    connect(m_loginDialog, &LoginDialog::signal_switch_reset, this, &MainWindow::slot_switch_reset);

}

void MainWindow::slot_switch_chat()
{
    m_chatDialog = new ChatDialog(this);
    setCentralWidget(m_chatDialog);
    m_chatDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setMinimumSize(QSize(1050, 700));
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
