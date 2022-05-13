#include "mainwindow.h"
#include <iostream>
#include <QApplication>


template<typename T>
class SINGLETON
{
public:
    static T* instance() {
        static T object;
        return &object;
    }

protected:
    SINGLETON<T>() = default;
    SINGLETON<T>(const SINGLETON<T> &other) = delete;
    SINGLETON<T>& operator=(const SINGLETON<T>&other) = delete;
};

class A : public SINGLETON<A>
{

};

void tests()
{
   Q_ASSERT(handovers::helpers::makeGeranCellId("534", "11") == "25736715" && "TEST 1 NOT PASSED");
}

int main(int argc, char *argv[])
{
    qDebug() << static_cast<HandoverType>(-5);
    tests();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
