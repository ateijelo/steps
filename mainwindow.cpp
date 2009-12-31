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

    Tile *t = new Tile(0,0,zoom);
    t->setPixmap(QPixmap("00-0_0.mgm.00x00.png"));
#if QT_VERSION >= 0x040600
    t->setScale(res);
#else
    t->scale(res,res);
#endif
    t->setPos(gt.Pixels2Meters(QPointF(0,0),zoom));
    tiles.append(t);
    scene->addItem(t);

    scene->setSceneRect(QRectF());

    connect(ui.mapView,SIGNAL(hadToPaint()),this,SLOT(mapViewHadToPaint()));
    connect(ui.mapView,SIGNAL(mouseMoved(QPoint)),this,SLOT(mapViewMouseMoved(QPoint)));
    connect(ui.zoom,SIGNAL(valueChanged(int)),this,SLOT(test()));
}

void MainWindow::mapViewHadToPaint()
{
//    QRectF drawArea = ui.mapView->mapToScene(ui.mapView->viewport()->rect()).boundingRect();
//    qDebug() << drawArea;
//    qDebug() << "tile boundaries: "
//             << gt.Meters2GoogleTile(drawArea.topLeft(),zoom)
//             << gt.Meters2GoogleTile(drawArea.bottomRight(),zoom);
}

void MainWindow::mapViewMouseMoved(const QPoint& p)
{
//    qDebug() << "mouse moved:" << gt.Meters2LatLon(ui.mapView->mapToScene(p));
}

void MainWindow::test()
{

}
