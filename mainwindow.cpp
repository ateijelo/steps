#include <QtDebug>
#include <QGraphicsPixmapItem>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,100,100);
    ui.view->setScene(scene);

//    QPixmap p("vedado.jpg");
//    qDebug() << p.width();
//    qDebug() << p.height();
//    QGraphicsPixmapItem *i = scene->addPixmap(QPixmap("vedado.jpg"));
//    qDebug() << i->isVisible();
//    ui.view->centerOn(i);
    QGraphicsItem *e = scene->addEllipse(0,0,80,80);
    ui.view->fitInView(e);
    qDebug() << ui.view->sceneRect();

}
