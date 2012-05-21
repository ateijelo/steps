#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QGraphicsRectItem>

#include "tile.h"

int qHash(const TileId& key)
{
    return qHash(QString("%1:%2:%3:%4")
                 .arg(key.type)
                 .arg(key.x)
                 .arg(key.y)
                 .arg(key.zoom));
}

QDebug& operator<<(QDebug& debug, const TileId& tile)
{
    debug << "TileId(" << tile.type << tile.x << tile.y << tile.zoom << ")";
    return debug;
}

Tile::Tile(QString type, int x, int y, int zoom)
        : _type(type), _x(x), _y(y), _zoom(zoom)
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
    qDebug() << "deleted Tile:" << _type << _x << _y
            << _zoom << _current_zoom;
}

TileCoords Tile::coords() const
{
    return TileCoords(_x,_y);
}

QString Tile::tileId() const
{
    return QString("type=%1 x=%2 y=%3 z=%4 cz=%5")
            .arg(_type).arg(_x).arg(_y).arg(_zoom).arg(_current_zoom);
}

QString Tile::tileKey(QString type, int x, int y, int z)
{
    return QString("%1:%2:%3:%4").arg(type).arg(x).arg(y).arg(z);
}

QString Tile::tileType() const
{
    return _type;
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

    int dz = _zoom - z;
    int w = 256 >> dz;
    int rx = _x % (1 << dz);
    int ry = _y % (1 << dz);
    if (dz > 8)
    {
        w = 1;
        rx = rx >> (dz - 8);
        ry = ry >> (dz - 8);
    }

    setPixmap(pixmap.copy(rx*w,ry*w,w,w).scaled(256,256));
    _current_zoom = z;
    //update();
}
