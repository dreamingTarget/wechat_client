#include "mainwindow.h"
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

}
