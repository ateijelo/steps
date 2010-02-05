#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

class MainScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        void wheelEvent(QGraphicsSceneWheelEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    signals:
        void zoomIn();
        void zoomOut();
};

#endif // MAINSCENE_H
