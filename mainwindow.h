#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>

#include "ui_mainwindow.h"
#include "preferences.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void updateLatLonLabels(const QPointF& latLon);

    private:
        Ui::MainWindow ui;
        QSlider zoomSlider;
        QRadioButton mapOption;
        QRadioButton satOption;
        QRadioButton hybOption;
        QCheckBox showLatLonAsToolTip;
        QLabel latLabel;
        QLabel lonLabel;
        Preferences *preferences;

    private slots:
        void openCacheDirectory();
};

#endif
