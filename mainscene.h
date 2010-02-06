#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

class MainScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    signals:
        void mouseMoved(const QPointF& pos);
};

#endif // MAINSCENE_H
