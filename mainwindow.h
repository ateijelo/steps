#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QLabel>
#include <QSlider>

#include "ui_mainwindow.h"
#include "geotools.h"
#include "tilemanager.h"
#include "tile.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    private slots:
        void updateLatLonLabels(const QPointF& latLon);

    private:
        Ui::MainWindow ui;
        QSlider zoomSlider;
        QRadioButton mapOption;
        QRadioButton satOption;
        QRadioButton hybOption;
        QLabel latLabel;
        QLabel lonLabel;

    private slots:
        void openCacheDirectory();
};

#endif
