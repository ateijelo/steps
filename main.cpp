/*
 * main.cpp -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        settings.sync();
    }

    MainWindow m;
    m.setWindowIcon(QIcon(":/icon.svg"));
    m.show();

    return app.exec();
}
