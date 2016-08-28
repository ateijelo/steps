/*
 * mainwindow.h -- part of Steps, a simple maps app
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QSignalMapper>
#include <QShortcut>

#include "ui_mainwindow.h"
#include "geotools.h"
#include "tile.h"
#include "preferences.h"
#include "debugdialog.h"
#include "pathsdockmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    protected:
        void closeEvent(QCloseEvent *event);

    private:
//        void openCacheDirectory();
//        void updateCacheDirectory(QString path);
//        void updateLatLonLabels(const QPointF& latLon);
        void aboutDialog();
        void newPath();

    private:
        void openMBTiles();
        void loadMBTilesFile(const QString& path);
//        void updateCacheStyles();
        //void updateRecents(QString top=QString());
        void loadRecents();
        void addRecent(const QString& path);
        void openRecentPath(int index);

        Ui::MainWindow ui;
        QStringList recentPaths;
        QSet<QAction*> recentPathActions;
//        QSignalMapper recentsMapper;
//        QSlider zoomSlider;
//        QCheckBox showLatLonAsToolTip;
//        QLabel latLabel;
//        QLabel lonLabel;
//        QComboBox cacheStyles;
        Preferences *preferences;
        DebugDialog *debugDialog;
        QShortcut *debugShortcut;
        PathsDockModel *pathsModel;

    private:
};

#endif
