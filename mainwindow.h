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

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::MainWindow ui;
        QList<QAction*> recentPaths;
        QSignalMapper recentsMapper;
        QSlider zoomSlider;
        QCheckBox showLatLonAsToolTip;
        QLabel latLabel;
        QLabel lonLabel;
        QComboBox cacheStyles;
        Preferences *preferences;
        DebugDialog *debugDialog;
        QShortcut *debugShortcut;

    private slots:
        void openCacheDirectory();
        void updateCacheDirectory(QString path);
        void updateLatLonLabels(const QPointF& latLon);
        void aboutDialog();

    private:
        void updateCacheStyles();
        void updateRecents(QString top=QString());
};

#endif
