#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsSceneWheelEvent>
#include <QGraphicsScene>

class MainScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        void wheelEvent(QGraphicsSceneWheelEvent *event);

    signals:
        void zoomIn();
        void zoomOut();
};

#endif // MAINSCENE_H
