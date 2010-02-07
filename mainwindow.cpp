#include <QtDebug>
#include <QFileDialog>
#include <QSettings>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mgmreader.h"
#include "mainscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

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

    connect(&mapOption, SIGNAL(clicked()), ui.mapView, SLOT(setMapStyle()));
    connect(&satOption, SIGNAL(clicked()), ui.mapView, SLOT(setSatStyle()));
    connect(&hybOption, SIGNAL(clicked()), ui.mapView, SLOT(setHybStyle()));

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
    connect(ui.mapView,SIGNAL(canZoomIn(bool)),ui.zoomInAction,SLOT(setEnabled(bool)));
    connect(ui.mapView,SIGNAL(canZoomOut(bool)),ui.zoomOutAction,SLOT(setEnabled(bool)));
    connect(ui.mapView,SIGNAL(zoomChanged(int)),&zoomSlider,SLOT(setValue(int)));
    connect(ui.mapView,SIGNAL(mouseMoved(QPointF)),this,SLOT(updateLatLonLabels(QPointF)));

    ui.zoomInAction->setShortcut(QKeySequence::ZoomIn);
    ui.zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QSettings settings;
    int tileStyle = settings.value("TileStyle", TILE_STYLE_MAP).toInt();
    switch(tileStyle)
    {
        case TILE_STYLE_SAT:
            satOption.setChecked(true);
            break;
        case TILE_STYLE_HYB:
            hybOption.setChecked(true);
            break;
        case TILE_STYLE_MAP:
        default:
            mapOption.setChecked(true);
            break;
    }
    settings.value("TileStyle", tileStyle);
    ui.mapView->setZoomLevel(0);
}

void MainWindow::openCacheDirectory()
{
    QSettings settings;
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Open Cache Directory",
                                                     settings.value("CachePath","").toString());
    if (path.compare("") != 0)
    {
        settings.setValue("CachePath",path);
    }
}

void MainWindow::updateLatLonLabels(const QPointF& latLon)
{
    latLabel.setText(QString("Lat: %1").arg(latLon.y()));
    lonLabel.setText(QString("Lon: %1").arg(latLon.x()));
}
