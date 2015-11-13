#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("ateijelo");
    app.setOrganizationDomain("ateijelo.com");
    app.setApplicationName("Steps");
    //app.setApplicationVersion("14.02");

    MainWindow m;
    m.setWindowIcon(QIcon(":/icon.svg"));
    m.show();

    return app.exec();
}
