#include <QtDebug>
#include <QtEndian>
#include <QGraphicsRectItem>
#include <QFile>
#include <QPainter>
#include <QSettings>

#include "tile.h"

Tile::Tile(int tileStyle, int x, int y, int zoom)
        : x(x), y(y), zoom(zoom)
{
    //setTransformationMode(Qt::SmoothTransformation);
    QPixmap result;
    QString filename;
    QGraphicsRectItem *r = new QGraphicsRectItem(0,0,256,256);
    r->setPen(Qt::NoPen);
    r->setParentItem(this);
    //QGraphicsTextItem *t = new QGraphicsTextItem(QString("(%1,%2,%3)").arg(x).arg(y).arg(zoom));
    //t->setParentItem(this);

    switch(tileStyle)
    {
        case TILE_STYLE_MAP:
            filename = getMapTileFileName(x, y, zoom);
            result = loadTile(filename, x, y);
            break;
        case TILE_STYLE_SAT:
            filename = getSatTileFileName(x, y, zoom);
            result = loadTile(filename, x, y);
            break;
        case TILE_STYLE_HYB:
            filename = getSatTileFileName(x, y, zoom);
            result = loadTile(filename, x, y);

            filename = getHybTileFileName(x, y, zoom);
            QPixmap hyb = loadTile(filename, x, y);
            if (result.isNull())
            {
                result = hyb;
            }
            else
            {
                QPainter painter(&result);
                painter.drawPixmap(0, 0, hyb.width(), hyb.height(), hyb);
                painter.end();
            }
            break;
    }

    setPixmap(result);
    setZValue(zoom);
}

QPixmap Tile::loadTile(QString filename, int x, int y)
{
    int in_x = x & 7;
    int in_y = y & 7;
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
            return 0;
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
                return 0;
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
                delete data;

                return p;
            }
        }
    }
    else
    {
        //qDebug() << "error opening " << filename << " for reading";
    }
    return 0;
}

QString Tile::getTileFileName(QString filenameFormat, int x, int y, int zoom)
{
    int mgm_x = x >> 3;
    int mgm_y = y >> 3;
    QSettings settings;
    return filenameFormat
            .arg(settings.value("CachePath","").toString())
            .arg(zoom)
            .arg(mgm_x)
            .arg(mgm_y);
}

QString Tile::getMapTileFileName(int x, int y, int zoom)
{
    return getTileFileName("%1/GoogleMap_%2/%3_%4.mgm", x, y, zoom);
}

QString Tile::getSatTileFileName(int x, int y, int zoom)
{
    return getTileFileName("%1/GoogleSat_%2/%3_%4.mgm", x, y, zoom);
}

QString Tile::getHybTileFileName(int x, int y, int zoom)
{
    return getTileFileName("%1/GoogleHyb_%2/%3_%4.mgm", x, y, zoom);
}

Tile::~Tile()
{
    //qDebug() << "deleted Tile:" << x << y << zoom;
}
