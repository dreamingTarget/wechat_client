#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_loginDialog = new LoginDialog;
    setCentralWidget(m_loginDialog);
    m_loginDialog->show();

    connect(m_loginDialog, &LoginDialog::signal_switch_register, this, &MainWindow::slot_switch_register);

    m_registerDialog = new RegisterDialog;

}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_loginDialog)
    {
        delete m_loginDialog;
        m_loginDialog = nullptr;
    }

    if (m_registerDialog)
    {
        delete m_registerDialog;
        m_registerDialog = nullptr;
    }
}

void MainWindow::slot_switch_register()
{
    m_loginDialog->hide();
    setCentralWidget(m_registerDialog);
    m_registerDialog->show();
}
