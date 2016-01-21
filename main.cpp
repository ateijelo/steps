#include <QApplication>

#include "mainwindow.h"
#include "constants.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("ateijelo");
    app.setOrganizationDomain("ateijelo.com");
    app.setApplicationName("Steps");
    //app.setApplicationVersion("14.02");

    if (app.arguments().size() > 1)
    {
        QSettings settings;
        settings.setValue(SettingsKeys::MBTilesPath, app.arguments().at(1));
    }

    MainWindow m;
    m.setWindowIcon(QIcon(":/icon.svg"));
    m.show();

    return app.exec();
}
