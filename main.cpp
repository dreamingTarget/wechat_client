#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFile>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    QFile file(":/style/stylesheet.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromLatin1(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
        qDebug("file open success");
    } else {
        qDebug("file open failed");
    }

    MainWindow w;
    w.show();
    return a.exec();    
}
