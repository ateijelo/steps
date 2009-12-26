#include <QtDebug>
#include <QWheelEvent>

#include "mapview.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
}

bool MapView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *e = (QWheelEvent*)(event);
        //qDebug() << e->orientation() << e->delta();
    }
    return false;
}
