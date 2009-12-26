#include <QtDebug>
#include <QWheelEvent>

#include "mapview.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
}

bool MapView::viewportEvent(QEvent *event)
{
    return QGraphicsView::viewportEvent(event);
}
