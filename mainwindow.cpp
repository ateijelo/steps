#include <QtDebug>
#include <QFileDialog>
#include <QSettings>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mgmreader.h"
#include "mainscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), angle(0)
{
    ui.setupUi(this);

    //scene = new QGraphicsScene(this);
    scene = new MainScene();
    connect(scene,SIGNAL(zoomIn()),this,SLOT(zoomIn()));
    connect(scene,SIGNAL(zoomOut()),this,SLOT(zoomOut()));

    ui.mapView->setScene(scene);
    ui.mapView->setSceneRect(QRectF(gt.Pixels2Meters(QPointF(  0,  0),0),
                                    gt.Pixels2Meters(QPointF(256,256),0)) );

    center = QPointF(-82.38,23.13);

    zoomSlider.setMinimum(0);
    zoomSlider.setMaximum(18);
    zoomSlider.setOrientation(Qt::Horizontal);
    zoomSlider.setFixedWidth(30+18*4);
    zoomSlider.setTickInterval(1);
    zoomSlider.setTickPosition(QSlider::TicksBelow);

    ui.toolBar->insertWidget(ui.zoomInAction, &zoomSlider);
    connect(&zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setZoomLevel(int)));

    mapOption.setChecked(true);
    mapOption.setText("&Maps");
    satOption.setText("&Satellite");
    hybOption.setText("&Hybrid");

    ui.toolBar->insertSeparator(0);
    ui.toolBar->insertWidget(0, &mapOption);
    ui.toolBar->insertWidget(0, &satOption);
    ui.toolBar->insertWidget(0, &hybOption);

    connect(&mapOption, SIGNAL(clicked()), this, SLOT(setMapStyle()));
    connect(&satOption, SIGNAL(clicked()), this, SLOT(setSatStyle()));
    connect(&hybOption, SIGNAL(clicked()), this, SLOT(setHybStyle()));

    latLabel.setFixedWidth(90);
    lonLabel.setFixedWidth(90);

    ui.toolBar->insertSeparator(0);
    ui.toolBar->insertWidget(0, &latLabel);
    ui.toolBar->insertWidget(0, &lonLabel);

    //double res = gt.resolution(zoom);

    //ui.mapView->scale(1/res, 1/res);

//    Tile *t = new Tile(0,0,zoom);
//    //t->setPixmap(QPixmap("00-0_0.mgm.00x00.png"));
//#if QT_VERSION >= 0x040600
//    t->setScale(res);
//#else
//    t->scale(res,res);
//#endif
//    t->setPos(gt.Pixels2Meters(QPointF(0,0),zoom));
//    tiles.append(t);
//    scene->addItem(t);
    Tile *t = new Tile(TILE_STYLE_MAP,0,0,0);
    t->setPixmap(QPixmap("00-0_0.mgm.00x00.png"));
    displayNewTile(t,0,0,0);
    //t->setZValue(0);

    tm.setTileStyle(TILE_STYLE_MAP);

    connect(&tm,SIGNAL(tileCreated(Tile*,int,int,int)),this,SLOT(displayNewTile(Tile*,int,int,int)));
    connect(ui.mapView,SIGNAL(hadToPaint()),this,SLOT(mapViewHadToPaint()));
    connect(ui.mapView,SIGNAL(mouseMoved(QPoint)),this,SLOT(mapViewMouseMoved(QPoint)));
    connect(ui.zoomInAction,SIGNAL(triggered()),this,SLOT(zoomIn()));
    connect(ui.zoomOutAction,SIGNAL(triggered()),this,SLOT(zoomOut()));
    connect(ui.rotRightAction,SIGNAL(triggered()),this,SLOT(rotRight()));
    connect(ui.rotLeftAction,SIGNAL(triggered()),this,SLOT(rotLeft()));
    connect(ui.openAction,SIGNAL(triggered()),this,SLOT(openCacheDirectory()));

    ui.zoomInAction->setShortcut(QKeySequence::ZoomIn);
    ui.zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    setZoomLevel(12);
    ui.mapView->centerOn(gt.LatLon2Meters(center));

    updateLatLonLabels(center);
}

void MainWindow::openCacheDirectory()
{
    QSettings settings;
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Open Cache Directory",
                                                     settings.value("CachePath","").toString());
    settings.setValue("CachePath",path);
}

void MainWindow::setZoomLevel(int zoom)
{
    QPoint lastCursorPos = ui.mapView->mapFromScene(gt.LatLon2Meters(lastLatLon));

    if (zoom < 0)
        zoom = 0;
    if (zoom > 18)
        zoom = 18;
    if (zoom == 0)
        ui.zoomOutAction->setEnabled(false);
    else
        ui.zoomOutAction->setEnabled(true);
    if (zoom == 18)
        ui.zoomInAction->setEnabled(false);
    else
        ui.zoomInAction->setEnabled(true);
    tm.clear();
    ui.mapView->resetTransform();
    ui.mapView->rotate(angle);
    double res = gt.resolution(zoom);
    ui.mapView->scale(1/res,1/res);
    this->zoom = zoom;

    updateLatLonLabels(gt.Meters2LatLon(ui.mapView->mapToScene(lastCursorPos)));
    zoomSlider.setValue(zoom);
}

void MainWindow::setMapStyle()
{
    tm.setTileStyle(TILE_STYLE_MAP);
    tm.clear();
    mapViewHadToPaint();
}

void MainWindow::setSatStyle()
{
    tm.setTileStyle(TILE_STYLE_SAT);
    tm.clear();
    mapViewHadToPaint();
}

void MainWindow::setHybStyle()
{
    tm.setTileStyle(TILE_STYLE_HYB);
    tm.clear();
    mapViewHadToPaint();
}

void MainWindow::zoomIn()
{
    setZoomLevel(zoom + 1);
}

void MainWindow::zoomOut()
{
    setZoomLevel(zoom - 1);
}

void MainWindow::rotRight()
{
    ui.mapView->rotate(15);
    angle = (angle+15 > 360)? angle - 345 : angle + 15;
}

void MainWindow::rotLeft()
{
    ui.mapView->rotate(-15);
    angle = (angle-15 < 0)? angle + 345 : angle - 15;
}

void MainWindow::displayNewTile(Tile *t, int x, int y, int zoom)
{
    t->resetTransform();
    double res = gt.resolution(zoom);
#if QT_VERSION >= 0x040600
    t->setScale(res);
#else
    t->scale(res,res);
#endif
    t->setPos(gt.GoogleTile2Meters(x,y,zoom));
    scene->addItem(t);
}

void MainWindow::mapViewHadToPaint()
{
    //QRectF drawArea = ui.mapView->mapToScene(ui.mapView->viewport()->rect().adjusted(+180,+180,-180,-180)).boundingRect();
    QRectF drawArea = ui.mapView->mapToScene(ui.mapView->viewport()->rect().adjusted(-20,-20,20,20)).boundingRect();
    QPoint tl = gt.Meters2GoogleTile(drawArea.topLeft(),zoom);
    QPoint br = gt.Meters2GoogleTile(drawArea.bottomRight(),zoom);
    tm.setRegion(QRect(tl,br),zoom);
}

void MainWindow::updateLatLonLabels(QPointF latLon)
{
    latLabel.setText(QString("Lat: %1").arg(latLon.x()));
    lonLabel.setText(QString("Lon: %1").arg(latLon.y()));

    lastLatLon = latLon;
}

void MainWindow::mapViewMouseMoved(const QPoint& p)
{
    QPointF latLon = gt.Meters2LatLon(ui.mapView->mapToScene(p));
    updateLatLonLabels(latLon);
}
