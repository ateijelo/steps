#include <QtDebug>

#include "mainscene.h"

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseMoved(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
    //qDebug() << "MainScene::mouseMoveEvent" << event->scenePos();
}
