#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle(QString::fromUtf8("V-Traco"));

    w.show();
    return a.exec();
}
