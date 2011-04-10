#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QGraphicsRectItem>

#include "tile.h"

Tile::Tile(int x, int y, int zoom)
        : _x(x), _y(y), _zoom(zoom)
{
//    setTransformationMode(Qt::SmoothTransformation);
//    QGraphicsRectItem *r = new QGraphicsRectItem(0,0,256,256);
//    r->setParentItem(this);
//    QGraphicsTextItem *t = new QGraphicsTextItem(QString("(%1,%2,%3)").arg(x).arg(y).arg(zoom));
//    t->setParentItem(this);
    setAcceptHoverEvents(true);
    _current_zoom = -1;
}

Tile::~Tile()
{
    qDebug() << "deleted Tile:" << _x << _y << _zoom << _current_zoom;
}

TileCoords Tile::coords() const
{
    return TileCoords(_x,_y);
}

QString Tile::tileId() const
{
    return QString("x=%1 y=%2 z=%3 cz=%4")
            .arg(_x).arg(_y).arg(_zoom).arg(_current_zoom);
}

QString Tile::tileKey(QString type, int x, int y, int z)
{
    return QString("%1:%2:%3:%4").arg(type).arg(x).arg(y).arg(z);
}

int Tile::tileX() const
{
    return _x;
}

int Tile::tileY() const
{
    return _y;
}

int Tile::zoom() const
{
    return _zoom;
}

int Tile::currentZoom() const
{
    return _current_zoom;
}

bool Tile::isWithin(int x, int y, int z) const
{
    if (z > _zoom) return false;
    return (x == _x >> (_zoom - z)) && (y == _y >> (_zoom - z));
}

void Tile::loadPixmap(const QPixmap &pixmap, int z)
{
    if (z > _zoom) return;
    if (z < _current_zoom) return;

    int w = 256 >> (_zoom - z);
    if (w == 0) w = 1;
    int rx = _x % (1 << (_zoom - z));
    int ry = _y % (1 << (_zoom - z));

    setPixmap(pixmap.copy(rx*w,ry*w,w,w).scaled(256,256));
    _current_zoom = z;
    //update();
}
