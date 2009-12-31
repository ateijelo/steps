#include <QtDebug>

#include "tile.h"

Tile::Tile(int x, int y, int zoom)
        : x(x), y(y), zoom(zoom)
{
    qDebug() << "new Tile:" << x << y << zoom;
}

Tile::~Tile()
{
    qDebug() << "deleted Tile:" << x << y << zoom;
}
