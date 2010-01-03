#include <QtDebug>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mgmreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), angle(0)
{
    ui.setupUi(this);

    scene = new QGraphicsScene(this);
    ui.mapView->setScene(scene);
    ui.mapView->setSceneRect(QRectF(gt.Pixels2Meters(QPointF(  0,  0),0),
                                    gt.Pixels2Meters(QPointF(256,256),0)) );

    center = QPointF(-82.38,23.13);

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

    connect(&tm,SIGNAL(tileCreated(Tile*,int,int,int)),this,SLOT(displayNewTile(Tile*,int,int,int)));
    connect(ui.mapView,SIGNAL(hadToPaint()),this,SLOT(mapViewHadToPaint()));
    connect(ui.mapView,SIGNAL(mouseMoved(QPoint)),this,SLOT(mapViewMouseMoved(QPoint)));
    connect(ui.zoomInAction,SIGNAL(triggered()),this,SLOT(zoomIn()));
    connect(ui.zoomOutAction,SIGNAL(triggered()),this,SLOT(zoomOut()));
    connect(ui.rotRightAction,SIGNAL(triggered()),this,SLOT(rotRight()));
    connect(ui.rotLeftAction,SIGNAL(triggered()),this,SLOT(rotLeft()));

    setZoomLevel(12);
    qDebug() << gt.LatLon2Meters(center);
    ui.mapView->centerOn(gt.LatLon2Meters(center));
}

void MainWindow::setZoomLevel(int zoom)
{
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
    ui.mapView->resetTransform();
    ui.mapView->rotate(angle);
    tm.clear();
    ui.mapView->scale(1/gt.resolution(zoom),1/gt.resolution(zoom));
    this->zoom = zoom;
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
    t->setScale(gt.resolution(zoom));
    t->setPos(gt.GoogleTile2Meters(x,y,zoom));
    scene->addItem(t);
}

void MainWindow::mapViewHadToPaint()
{
    QRectF drawArea = ui.mapView->mapToScene(ui.mapView->viewport()->rect().adjusted(-20,-20,20,20)).boundingRect();
    QPoint tl = gt.Meters2GoogleTile(drawArea.topLeft(),zoom);
    QPoint br = gt.Meters2GoogleTile(drawArea.bottomRight(),zoom);
    tm.setRegion(QRect(tl,br),zoom);
}

void MainWindow::mapViewMouseMoved(const QPoint& p)
{
    qDebug() << "mouse moved:" << gt.Meters2LatLon(ui.mapView->mapToScene(p));
}
