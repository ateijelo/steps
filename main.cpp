#include <QtDebug>
#include <QApplication>

#include <QRectF>

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

    QRectF r1(100,100,0,0);
    QRectF r2(50,50,100,100);

    qDebug() << r2.contains(r1) << endl;

    return app.exec();
}
