#include <QtDebug>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mgmreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    scene = new QGraphicsScene(this);
    ui.mapView->setScene(scene);
    ui.mapView->setSceneRect(QRectF(gt.Pixels2Meters(QPointF(  0,  0),0),
                                    gt.Pixels2Meters(QPointF(256,256),0)) );

    center = QPointF(-82.38,23.13);
    zoom = 0;
    if (zoom == 0)
        ui.zoomOut->setEnabled(false);
    if (zoom == 19)
        ui.zoomIn->setEnabled(false);

    double res = gt.resolution(zoom);

    ui.mapView->scale(1/res/2, 1/res/2);

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

    tm = new TileManager();

    connect(tm,SIGNAL(tileCreated(Tile*,int,int,int)),this,SLOT(displayNewTile(Tile*,int,int,int)));
    connect(ui.mapView,SIGNAL(hadToPaint()),this,SLOT(mapViewHadToPaint()));
    connect(ui.mapView,SIGNAL(mouseMoved(QPoint)),this,SLOT(mapViewMouseMoved(QPoint)));
    connect(ui.zoom,SIGNAL(valueChanged(int)),this,SLOT(test()));
    connect(ui.zoomIn,SIGNAL(clicked()),this,SLOT(zoomIn()));
    connect(ui.zoomOut,SIGNAL(clicked()),this,SLOT(zoomOut()));
}

void MainWindow::zoomIn()
{
    tm->clear();
    zoom++;
    if (zoom == 19)
        ui.zoomIn->setEnabled(false);
    ui.zoomOut->setEnabled(true);
    ui.mapView->scale(2.0,2.0);
}

void MainWindow::zoomOut()
{
    tm->clear();
    zoom--;
    if (zoom == 0)
        ui.zoomOut->setEnabled(false);
    ui.zoomIn->setEnabled(true);
    ui.mapView->scale(0.5,0.5);
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
    QRectF drawArea = ui.mapView->mapToScene(ui.mapView->viewport()->rect().adjusted(64,64,-64,-64)).boundingRect();
    QPoint tl = gt.Meters2GoogleTile(drawArea.topLeft(),zoom);
    QPoint br = gt.Meters2GoogleTile(drawArea.bottomRight(),zoom);
    tm->setRegion(QRect(tl,br),zoom);
}

void MainWindow::mapViewMouseMoved(const QPoint& p)
{
//    qDebug() << "mouse moved:" << gt.Meters2LatLon(ui.mapView->mapToScene(p));
}

void MainWindow::test()
{

}
