#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSettings>

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

    QString fileName = "config.ini";
    QString appPath = QCoreApplication::applicationDirPath();
    QString configPath = QDir::toNativeSeparators(appPath + QDir::separator() + fileName);
    QSettings settings(configPath, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    MainWindow w;
    w.show();
    return a.exec();
}
