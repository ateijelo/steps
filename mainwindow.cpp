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

    center = QPointF(-82.38,23.13);
    zoom = 0;

    double res = gt.resolution(zoom);

    ui.mapView->scale(1/res, 1/res);

    Tile *t = new Tile();
    t->setPixmap(QPixmap("00-0_0.mgm.00x00.png"));
#if QT_VERSION >= 0x040600
    t->setScale(res);
#else
    t->scale(res,res);
#endif
    tiles.append(t);
    scene->addItem(t);

    scene->setSceneRect(QRectF());

    connect(ui.mapView,SIGNAL(hadToPaint()),this,SLOT(mapViewHadToPaint()));
    connect(ui.lat,SIGNAL(valueChanged(double)),this,SLOT(test()));
    connect(ui.lon,SIGNAL(valueChanged(double)),this,SLOT(test()));
    connect(ui.zoom,SIGNAL(valueChanged(int)),this,SLOT(test()));
}

void MainWindow::mapViewHadToPaint()
{
    // If the view can be rotated, drawArea should be computed
    // as follows:
    //   QRect viewRect(ui.mapView->viewport()->rect());
    //   QPolygonF mappedToScene(ui.mapView->mapToScene(viewRect));
    //   QRectF sceneBounds(mappedToScene.boundingRect());
    //   QRect drawArea(ui.mapView->mapFromScene(sceneBounds).boundingRect());
    //QRect drawArea(ui.mapView->viewport()->rect());
    //qDebug() << drawArea;
    //drawArea.moveTo(center.x()-drawArea.width()/2,center.y()-drawArea.height()/2);
    //qDebug() << drawArea;
    //qDebug() << ui.mapView->mapToScene(ui.mapView->viewport()->rect()).boundingRect();
}

void MainWindow::test()
{
    double mx,my;
    gt.LatLon2Meters(ui.lat->value(),ui.lon->value(),&mx,&my);
    ui.mx->setValue(mx);
    ui.my->setValue(my);
    double px,py;
    gt.Meters2Pixels(mx,my,ui.zoom->value(),&px,&py);
    ui.px->setValue(px);
    ui.py->setValue(py);
    int tx,ty;
    gt.Pixels2TMSTile(px,py,&tx,&ty);
    //ui.tx->setValue(tx);
    //ui.ty->setValue(ty);
    int gx,gy;
    gt.TMS2Google(tx,ty,ui.zoom->value(),&gx,&gy);
    //ui.gx->setValue(gx);
    //ui.gy->setValue(gy);
}
