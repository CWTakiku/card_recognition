#include "mainwindow.h"
#include <QApplication>
#include<service.cpp>
#include<picture.cpp>
#include<handle.cpp>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
