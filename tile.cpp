#include <QtDebug>
#include <QtEndian>
#include <QGraphicsRectItem>
#include <QFile>
#include <QPainter>
#include <QSettings>

#include "tile.h"
#include "constants.h"

Tile::Tile(int x, int y, int zoom)
        : x(x), y(y), zoom(zoom)
{
    //setTransformationMode(Qt::SmoothTransformation);
    //QGraphicsRectItem *r = new QGraphicsRectItem(0,0,256,256);
    //r->setParentItem(this);
    //QGraphicsTextItem *t = new QGraphicsTextItem(QString("(%1,%2,%3)").arg(x).arg(y).arg(zoom));
    //t->setParentItem(this);
    setAcceptHoverEvents(true);

    QSettings settings;
    QString tileStyle = settings.value(SettingsKeys::MapType, MapTypes::GoogleMap).toString();
    QPixmap result = loadTile(tileStyle, x, y, zoom);
    if (tileStyle == MapTypes::GoogleHyb)
    {
        QPixmap sat = loadTile(MapTypes::GoogleSat, x, y, zoom);
        if (!sat.isNull())
        {
            QPainter painter(&sat);
            painter.drawPixmap(0, 0, result.width(), result.height(), result);
            painter.end();
            result = sat;
        }
    }

    setPixmap(result);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    //qDebug() << "Created Tile:" << x << y << zoom;
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

Tile::~Tile()
{
    //qDebug() << "Deleted Tile:" << x << y << zoom;
}

QPixmap Tile::loadTile(QString tileStyle, int x, int y, int zoom)
{
    int in_x = x & 7;
    int in_y = y & 7;
    QFile mgm(getTileFileName(tileStyle, x, y, zoom));
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

QString Tile::getTileFileName(QString tileStyle, int x, int y, int zoom)
{
    int mgm_x = x >> 3;
    int mgm_y = y >> 3;
    QSettings settings;
    return QString("%1/%2_%3/%4_%5.mgm")
            .arg(settings.value(SettingsKeys::CachePath,"").toString())
            .arg(tileStyle)
            .arg(zoom)
            .arg(mgm_x)
            .arg(mgm_y);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug() << "Tile::paint" << zoom;
    QGraphicsPixmapItem::paint(painter,option,widget);
}
