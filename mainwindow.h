#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>
#include <QComboBox>

#include "ui_mainwindow.h"
#include "geotools.h"
#include "tile.h"
#include "preferences.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::MainWindow ui;
        QSlider zoomSlider;
        QCheckBox showLatLonAsToolTip;
        QLabel latLabel;
        QLabel lonLabel;
        QComboBox cacheStyles;
        Preferences *preferences;

    private slots:
        void openCacheDirectory();
        void updateLatLonLabels(const QPointF& latLon);

    private:
        void updateCacheStyles();
};

#endif
