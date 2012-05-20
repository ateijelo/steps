#define QT_NO_DEBUG_OUTPUT
#include <QSettings>
#include <QtEndian>
#include <QtDebug>
#include <QFile>
#include <QDir>

#include "constants.h"
#include "disktask.h"

DiskTask::DiskTask(QObject *parent) :
    Task(parent)
{
}

void DiskTask::fetchTile(const QString &type, int x, int y, int zoom)
{
    store = false;
    tile_type = type;
    tile_x = x;
    tile_y = y;
    tile_zoom = zoom;
}

void DiskTask::storeTile(const TileId &tile, QByteArray data)
{
    storeTile(tile.type,tile.x,tile.y,tile.zoom,data);
}

void DiskTask::storeTile(const QString &type, int x, int y, int zoom, QByteArray data)
{
    store = true;
    tile_type = type;
    tile_x = x;
    tile_y = y;
    tile_zoom = zoom;
    store_data = data;
}

void DiskTask::readMgm()
{
    QString filename = getTileFileName(tile_type,tile_x,tile_y,tile_zoom);
    qDebug() << this << "started. Fetching" << tile_x << tile_y << "from" << filename;
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
        quint32 max_size = 0;
        no_tiles = qFromBigEndian(no_tiles);
        QList<QPoint> coords;
        QList<quint32> sizes;
        for (int i=0; i<no_tiles; i++)
        {
            quint8 tx,ty;
            r = mgm.read((char*)(&tx),1);
            r += mgm.read((char*)(&ty),1);
            r += mgm.read((char*)(&tile_end),4);
            if (r != 6)
            {
                qDebug() << "error reading tile entry " << i;
                return;
            }
            qDebug() << tx + ((tile_x >> 3) << 3) << ty + ((tile_y >> 3) << 3);
            coords.append(QPoint(tx + ((tile_x >> 3) << 3),ty + ((tile_y >> 3) << 3)));
            qDebug() << coords;
            tile_end = qFromBigEndian(tile_end);
            quint32 tile_size = tile_end - tile_start;
            if (tile_size > max_size)
                max_size = tile_size;
            sizes.append(tile_size);
            tile_start = tile_end;
        }
        mgm.seek(64*6+2);
        char *_data = new char[max_size];
        for (int i=0; i<no_tiles; i++)
        {
            quint32 tile_size = sizes.at(i);
            int tx = coords.at(i).x();
            int ty = coords.at(i).y();
            r = mgm.read(_data,tile_size);
            if (r != tile_size)
            {
                qDebug() << "error reading tile " << tx << "," << ty << "from file" << mgm.fileName();
                break;
            }
            else
            {
                QByteArray a;
                a.append(_data,tile_size);
                qDebug() << "emitting mgm tileData for " << tile_type << tx << ty << tile_zoom;
                emit tileData(tile_type,tx,ty,tile_zoom,a);
            }
        }
        delete[] _data;
    }
    else
    {
        qDebug() << "error opening " << filename << " for reading";
    }
}

void DiskTask::readSingleFile()
{
    QSettings settings;
    QDir d(settings.value(SettingsKeys::CachePath,"").toString());
    QFile f(d.absoluteFilePath(QString("cache/%1/%2/%3_%4").arg(tile_type).arg(tile_zoom).arg(tile_x).arg(tile_y)));
    qDebug() << "opening" << f.fileName();
    if (f.open(QIODevice::ReadOnly))
    {
        qDebug() << "   succeeded";
        emit tileData(tile_type,tile_x,tile_y,tile_zoom,f.readAll());
    }
    qDebug() << "   failed";
}

void DiskTask::work()
{
    //sleep(1);
    if (store)
    {
        qDebug() << "storing tile" << tile_type << tile_x << tile_y << tile_zoom;
        QSettings settings;
        QDir d(settings.value(SettingsKeys::CachePath,"").toString());
        d.mkpath(QString("cache/%1/%2").arg(tile_type).arg(tile_zoom));
        QFile f(d.absoluteFilePath(QString("cache/%1/%2/%3_%4").arg(tile_type).arg(tile_zoom).arg(tile_x).arg(tile_y)));
        if (f.open(QIODevice::WriteOnly))
        {
            f.write(store_data);
            f.close();
        }
    }
    else
    {
        qDebug() << "trying to read from single file" << tile_type << tile_x << tile_y << tile_zoom;
        readSingleFile();
        readMgm();
    }
    emit finished(this);
}

QString DiskTask::getTileFileName(QString tileStyle, int x, int y, int zoom)
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
