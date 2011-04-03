#include <QtDebug>
#include <QtEndian>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <unistd.h>

#include "constants.h"
#include "fetchtask.h"

FetchTask::FetchTask(const QString &tile_type, int x, int y, int zoom,
                     QObject *parent) :
    QObject(parent)
{
    this->tile_type = tile_type;
    this->tile_x = x;
    this->tile_y = y;
    this->tile_zoom = zoom;

    startEvent = QEvent::Type(QEvent::registerEventType());
}

void FetchTask::customEvent(QEvent *event)
{
    if (event->type() == startEvent)
    {
        state = Start;
        work();
    }
}

void FetchTask::start()
{
    qApp->postEvent(this,new QEvent(startEvent));
}

void FetchTask::work()
{
    // This is the place to do the real fetching work.
    // If the whole mess of threads and events I've brewed up to this
    // point works, here we can use QNetworkAccessManager and its family.
    // Pixmaps, however, should not be used here.
    // This isn't the best place for caching, though some nice optimizations
    // may be performed here. Pixmap caching should be handled downstream,
    // possibly by TileFetcher.
    // This isn't the place either to composite tiles that need it.
    // That should also happen later in the chain, closer to the
    // QGraphicsPixmapItem that each tile is.
    // For Google Hybrid two different fetch tasks should be spawned and
    // their result composited somewhere else.
    QPixmap p = loadTile(tile_type,tile_x,tile_y,tile_zoom);
}

QPixmap FetchTask::loadTile(QString tile_type, int x, int y, int zoom)
{
    int in_x = x & 7;
    int in_y = y & 7;
    QFile mgm(getTileFileName(tile_type, x, y, zoom));
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

QString FetchTask::getTileFileName(QString tileStyle, int x, int y, int zoom)
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
