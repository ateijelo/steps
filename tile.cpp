#include <QtDebug>
#include <QtEndian>
#include <QGraphicsRectItem>
#include <QFile>

#include "tile.h"

Tile::Tile(int x, int y, int zoom)
        : x(x), y(y), zoom(zoom)
{
    //setTransformationMode(Qt::SmoothTransformation);
    int mgm_x = x >> 3;
    int mgm_y = y >> 3;
    int in_x = x & 7;
    int in_y = y & 7;
    QString filename = QString("/Users/ateijelo/programs/qt4/qtgmaps/MGMapsCache/GoogleMap_%1/%2_%3.mgm")
                            .arg(zoom)
                            .arg(mgm_x)
                            .arg(mgm_y);
    QFile mgm(filename);
    if (mgm.open(QIODevice::ReadOnly))
    {
        quint64 r = 0;
        quint32 tile_start = 64*6 + 2;
        quint32 tile_end;
        quint16 no_tiles;
        r += mgm.read((char*)(&no_tiles),2);
        if (r != 2)
        {
            qDebug() << "error reading no_tiles";
            return;
        }
        no_tiles = qFromBigEndian(no_tiles);
        bool found = false;
        for (int i=0; i<no_tiles; i++)
        {
            quint8 tile_x,tile_y;
            r = mgm.read((char*)(&tile_x),1);
            r += mgm.read((char*)(&tile_y),1);
            r += mgm.read((char*)(&tile_end),4);
            if (r != 6)
            {
                qDebug() << "error reading tile entry " << i;
                return;
            }
            tile_end = qFromBigEndian(tile_end);
            if (tile_x == in_x && tile_y == in_y)
            {
                found = true;
                break;
            }
            tile_start = tile_end;
        }
        if (found)
        {
            quint32 tile_size = tile_end - tile_start;
            char *data = new char[tile_size];
            mgm.seek(tile_start);
            r = mgm.read(data,tile_size);
            if (r != tile_size)
            {
                qDebug() << "error reading tile " << x << "," << y << "data";
            }
            else
            {
                QPixmap p;
                p.loadFromData((uchar*)data,(uint)tile_size,0);
                setPixmap(p);
            }
            delete data;
        }
    }
}

Tile::~Tile()
{
    //qDebug() << "deleted Tile:" << x << y << zoom;
}
