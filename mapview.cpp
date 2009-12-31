#include <QtDebug>
#include <QWheelEvent>

#include "mapview.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
}

bool MapView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::Paint)
        emit hadToPaint();
    return QGraphicsView::viewportEvent(event);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event->pos());
    QGraphicsView::mouseMoveEvent(event);
}
