#include <QApplication>

#include "mainwindow.h"
#include "tilemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("ateijelo");
    app.setOrganizationDomain("com.ateijelo");
    app.setApplicationName("QtGMaps");
    app.setApplicationVersion("0.1");

    MainWindow m;
    m.show();

    return app.exec();
//    TileManager tm;
//
//    tm.setRegion(QRect(1,4,2,3),0);
//    tm.clear();
//    tm.setRegion(QRect(2,3,3,3),0);
}
