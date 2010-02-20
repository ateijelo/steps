#include <QtDebug>
#include <QFileDialog>
#include <QSettings>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mainscene.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    preferences = new Preferences(this);

    zoomSlider.setMinimum(0);
    zoomSlider.setMaximum(18);
    zoomSlider.setOrientation(Qt::Horizontal);
    zoomSlider.setFixedWidth(30+18*4);
    zoomSlider.setTickInterval(1);
    zoomSlider.setTickPosition(QSlider::TicksBelow);
    zoomSlider.setPageStep(1);

    ui.toolBar->insertWidget(ui.zoomInAction, &zoomSlider);
    connect(&zoomSlider, SIGNAL(valueChanged(int)), ui.mapView, SLOT(setZoomLevel(int)));

    mapOption.setText("&Maps");
    satOption.setText("&Satellite");
    hybOption.setText("&Hybrid");

    ui.toolBar->insertSeparator(0);
    ui.toolBar->insertWidget(0, &mapOption);
    ui.toolBar->insertWidget(0, &satOption);
    ui.toolBar->insertWidget(0, &hybOption);

    connect(&mapOption, SIGNAL(clicked()), ui.mapView, SLOT(setMapType2GoogleMap()));
    connect(&satOption, SIGNAL(clicked()), ui.mapView, SLOT(setMapType2GoogleSat()));
    connect(&hybOption, SIGNAL(clicked()), ui.mapView, SLOT(setMapType2GoogleHyb()));

    latLabel.setFixedWidth(90);
    lonLabel.setFixedWidth(90);

    ui.toolBar->insertSeparator(0);
    ui.toolBar->insertWidget(0, &latLabel);
    ui.toolBar->insertWidget(0, &lonLabel);

    connect(ui.zoomInAction,SIGNAL(triggered()),ui.mapView,SLOT(zoomIn()));
    connect(ui.zoomOutAction,SIGNAL(triggered()),ui.mapView,SLOT(zoomOut()));
    connect(ui.rotRightAction,SIGNAL(triggered()),ui.mapView,SLOT(rotRight()));
    connect(ui.rotLeftAction,SIGNAL(triggered()),ui.mapView,SLOT(rotLeft()));
    connect(ui.openAction,SIGNAL(triggered()),this,SLOT(openCacheDirectory()));
    connect(ui.action_Preferences, SIGNAL(triggered()), preferences, SLOT(show()));
    connect(ui.mapView,SIGNAL(canZoomIn(bool)),ui.zoomInAction,SLOT(setEnabled(bool)));
    connect(ui.mapView,SIGNAL(canZoomOut(bool)),ui.zoomOutAction,SLOT(setEnabled(bool)));
    connect(ui.mapView,SIGNAL(zoomChanged(int)),&zoomSlider,SLOT(setValue(int)));
    connect(ui.mapView,SIGNAL(mouseMoved(QPointF)),this,SLOT(updateLatLonLabels(QPointF)));

    ui.zoomInAction->setShortcut(QKeySequence::ZoomIn);
    ui.zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QSettings settings;

    QString tileStyle = settings.value(SettingsKeys::MapType, MapTypes::GoogleMap).toString();
    //settings.setValue(SettingsKeys::MapType, tileStyle);
    if (tileStyle == TILE_STYLE_SAT)
    {
        satOption.setChecked(true);
    }
    else if (tileStyle == TILE_STYLE_HYB)
    {
        hybOption.setChecked(true);
    }
    else //if (tileStyle == TILE_STYLE_MAP)
    {
        mapOption.setChecked(true);
    }

    ui.zoomInAction->setEnabled(ui.mapView->canZoomIn());
    ui.zoomOutAction->setEnabled(ui.mapView->canZoomOut());
    zoomSlider.setValue(ui.mapView->zoomLevel());
}

void MainWindow::openCacheDirectory()
{
    QSettings settings;
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Open Cache Directory",
                                                     settings.value(SettingsKeys::CachePath,"").toString());
    if (path.compare("") != 0)
    {
        settings.setValue(SettingsKeys::CachePath,path);
    }
}

void MainWindow::updateLatLonLabels(const QPointF& latLon)
{
    latLabel.setText(QString("Lat: %1").arg(latLon.y()));
    lonLabel.setText(QString("Lon: %1").arg(latLon.x()));
}
