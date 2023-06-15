#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //这是一行注释
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
