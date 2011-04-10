//#define QT_NO_DEBUG_OUTPUT

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

FetchTask::~FetchTask()
{
    qDebug() << this << "destroyed.";
}

void FetchTask::customEvent(QEvent *event)
{
    if (event->type() == startEvent)
    {
        state = Start;
        work();
        emit finished(this);
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
    // This isn't the place either to composite tiles that need it (GoogleHyb)
    // That should also happen later in the chain, closer to the
    // QGraphicsPixmapItem that each tile is.
    // For Google Hybrid two different fetch tasks should be spawned and
    // their result composited somewhere else.

    //sleep(1);
    //int in_x = tile_x & 7;
    //int in_y = tile_y & 7;
    QString filename = getTileFileName(tile_type,tile_x,tile_y,tile_zoom);
    qDebug() << this << "started. Fetching" << tile_x << tile_y << "from" << filename;
    QFile mgm(filename);
    if (mgm.open(QIODevice::ReadOnly))
    {
        int tiles_per_mgm = 64;
        mgm_tile_desc tiles[tiles_per_mgm];
        quint64 r = 0;
        quint32 tile_start = tiles_per_mgm*6 + 2;
        quint32 tile_end;
        quint16 no_tiles;
        r += mgm.read((char*)(&no_tiles),2);
        if (r != 2)
        {
            qDebug() << "error reading no_tiles";
            return;
        }
        no_tiles = qFromBigEndian(no_tiles);
        qDebug() << "  found" << no_tiles << "tiles";
        r = mgm.read((char*)tiles,sizeof(mgm_tile_desc)*no_tiles);
        if (r != sizeof(mgm_tile_desc)*no_tiles)
        {
            qDebug() << "error reading tile descriptions";
        }
        int x0 = (tile_x >> 3) << 3;
        int y0 = (tile_y >> 3) << 3;
        for (int i=0; i<no_tiles; i++)
        {
            mgm_tile_desc td = tiles[i];
            tile_start = (i==0)? tile_start : qFromBigEndian(tiles[i-1].end);
            tile_end = qFromBigEndian(td.end);
            quint32 tile_size = tile_end - tile_start;
            char *data = new char[tile_size];
            mgm.seek(tile_start);
            r = mgm.read(data,tile_size);
            if (r == tile_size)
            {
                QByteArray ba(data,tile_size);
                qDebug() << "  emit tileData" << tile_type
                        << x0 + td.x
                        << y0 + td.y
                        << tile_zoom
                        << tile_size << "bytes";
                emit tileData(tile_type,x0 + td.x,y0 + td.y,tile_zoom,ba);
            }
            else
            {
                qDebug() << "error reading tile " << td.x << "," << td.y;
            }
            tile_start = tile_end;
            delete[] data;
        }
    }
    else
    {
        //qDebug() << "error opening " << filename << " for reading";
    }
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
