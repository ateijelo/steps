#include <QtDebug>

#include "mainscene.h"

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseMoved(event->scenePos());
    //qDebug() << "MainScene::mouseMoveEvent" << event->scenePos();
}
