#include <QtDebug>

#include "mapview.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
}

bool MapView::viewportEvent(QEvent *event)
{
    qDebug() << event->type();
}
