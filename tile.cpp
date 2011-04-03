#include <QGraphicsRectItem>

#include "tile.h"

Tile::Tile(int x, int y, int zoom)
        : x(x), y(y), zoom(zoom)
{
    //setTransformationMode(Qt::SmoothTransformation);
    QGraphicsRectItem *r = new QGraphicsRectItem(0,0,256,256);
    r->setParentItem(this);
    QGraphicsTextItem *t = new QGraphicsTextItem(QString("(%1,%2,%3)").arg(x).arg(y).arg(zoom));
    t->setParentItem(this);
    //setAcceptHoverEvents(true);
}

Tile::~Tile()
{
    //qDebug() << "deleted Tile:" << x << y << zoom;
}
