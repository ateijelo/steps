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
    QGraphicsPixmapItem *i = scene->addPixmap(QPixmap("vedado.jpg"));
//    qDebug() << i->isVisible();
//    ui.view->centerOn(i);
    QGraphicsItem *e = scene->addEllipse(0,0,80,80,QPen(QBrush(Qt::black),2));
//    e->setVisible(true);
//    ui.view->fitInView(e);
    scene->setSceneRect(QRectF());
    qDebug() << ui.view->sceneRect();

}
