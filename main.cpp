#include <QApplication>

#include "mainwindow.h"
#include "tilemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //MainWindow m;
    //m.show();

    TileManager tm;

    tm.setRegion(QRect(0,0,2,2),0);
    tm.setRegion(QRect(1,4,2,3),0);
    tm.setRegion(QRect(4,4,3,3),0);
    tm.setRegion(QRect(2,3,4,3),0);

    //return app.exec();
}
