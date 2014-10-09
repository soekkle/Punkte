/*!
 *@autor soekkle
 *@date 17.02.14
 *@version 0.1
*/
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
