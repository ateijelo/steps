#include <QApplication>

#include "mainwindow.h"

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
}
