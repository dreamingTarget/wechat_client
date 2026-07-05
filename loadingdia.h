#ifndef LOADINGDIA_H
#define LOADINGDIA_H

#include <QDialog>

namespace Ui {
class LoadingDia;
}

class LoadingDia : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDia(QWidget *parent = nullptr);
    ~LoadingDia();

private:
    Ui::LoadingDia *ui;
};

#endif // LOADINGDIA_H
