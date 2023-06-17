#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);
    QFile qssFile(":/resources/qss/Aqua.qss");
    if(qssFile.open(QFile::ReadOnly)){
        a.setStyleSheet(qssFile.readAll());
    }
    qssFile.close();
    MainWindow w;
    w.show();
    return a.exec();
}
