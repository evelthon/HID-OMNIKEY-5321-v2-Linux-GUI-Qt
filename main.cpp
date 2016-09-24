#include "mainwindow.h"
#include <QApplication>
//#include "scard.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //would be nice to use this but we have a conversion to Qvariant.
    //for future investigation
//    a.setProperty('hContext', hContext)

    MainWindow w;
    w.show();





    return a.exec();
}
