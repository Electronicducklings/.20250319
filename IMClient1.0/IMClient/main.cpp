#include "login.h"

#include <QApplication>
#include"kernel.h"

Kernel* pKernel=nullptr;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Login w;
    //w.show();
    Kernel kerkel;

    pKernel=&kerkel;

    return a.exec();
}
