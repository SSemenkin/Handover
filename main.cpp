#include "mainwindow.h"
#include <iostream>
#include <QApplication>

void tests()
{
    qDebug() << handovers::helpers::makeGeranCellId("3074", "11");
}

int main(int argc, char *argv[])
{
    tests();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
