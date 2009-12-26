#include <QtDebug>
#include <QGraphicsPixmapItem>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    scene = new QGraphicsScene(this);
    ui.view->setScene(scene);

//    QPixmap p("vedado.jpg");
//    qDebug() << p.width();
//    qDebug() << p.height();
    for (int i=0; i<10; i++)
    {
        tiles.append(scene->addPixmap(QPixmap("vedado.jpg")));
        tiles.last()->setPos(4096*i,0);
    }
//    qDebug() << i->isVisible();
//    ui.view->centerOn(i);
    QGraphicsItem *e = scene->addEllipse(0,0,80,80,QPen(QBrush(Qt::black),2));
//    e->setVisible(true);
//    ui.view->fitInView(e);
    scene->setSceneRect(QRectF());
    qDebug() << ui.view->sceneRect();

}

void MainWindow::on_pushButton_clicked()
{
    if (tiles.isEmpty())
        return;
    QGraphicsPixmapItem *i = tiles.takeFirst();
    delete i;
}
