#include "loadingdia.h"
#include "ui_loadingdia.h"

#include <QMovie>

LoadingDia::LoadingDia(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingDia)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QMovie* movie = new QMovie(":/picture/loading.gif");
    movie->setScaledSize(QSize(70, 70));
    ui->label_loading->setMovie(movie);
    movie->start();
}

LoadingDia::~LoadingDia()
{
    delete ui;
}
