#include <QApplication>

#include "mainwindow.h"
#include "tilemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow m;
    m.show();

    return app.exec();
//    TileManager tm;
//
//    tm.setRegion(QRect(1,4,2,3),0);
//    tm.clear();
//    tm.setRegion(QRect(2,3,3,3),0);
}
