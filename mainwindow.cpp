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

    for (int i=0; i<3; i++)
    {
        tiles.append(scene->addPixmap(QPixmap("vedado.jpg")));
        tiles.last()->setPos(4096*i,0);
    }
    scene->setSceneRect(QRectF());
    qDebug() << ui.mapView->sceneRect();
    connect(ui.mapView,SIGNAL(hadToPaint()),this,SLOT(mapViewHadToPaint()));
    MGMReader::read("MGMapsCache/GoogleMap_17/4444_7115.mgm");
}

void MainWindow::mapViewHadToPaint()
{
    //qDebug() << ui.mapView->mapToScene(ui.mapView->viewport()->rect()).boundingRect();
}
