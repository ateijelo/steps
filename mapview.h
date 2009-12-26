#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QEvent>
#include <QGraphicsView>

class MapView : public QGraphicsView
{
    Q_OBJECT
    public:
        MapView(QWidget *parent=NULL);

        bool viewportEvent(QEvent *event);
};

#endif
