#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include "memcache.h"

MemCache::MemCache(QObject *parent) :
    QObject(parent)
{
    cache.setMaxCost(50*1024*1024);
}

QByteArray MemCache::getTileData(const QString &type, int x, int y, int zoom) const
{
    TileId t(type,x,y,zoom);
    return getTileData(t);
}

QByteArray MemCache::getTileData(const TileId &id) const
{
    TileWrap *a = cache.object(id);
    if (a == 0)
    {
        qDebug() << "MemCache miss" << id;
        return QByteArray();
    }
    qDebug() << "MemCache hit" << id;
    return *(a->data);
}

bool MemCache::contains(const QString &type, int x, int y, int zoom) const
{
    TileId t(type,x,y,zoom);
    return contains(t);
}

bool MemCache::contains(const TileId &id) const
{
    return cache.contains(id);
}

void MemCache::insert(const QString &type, int x, int y, int zoom, const QByteArray& data)
{
    TileId t(type,x,y,zoom);
    insert(t,data);
}

void MemCache::insert(const TileId &id, const QByteArray& data)
{
    if (contains(id))
        return;
    TileWrap *w = new TileWrap(data,id);
    cache.insert(id,w,data.size());
    qDebug() << "MemCache.cache.count:" << cache.count();
    qDebug() << "MemCache.cache.maxCost:" << cache.maxCost();
    qDebug() << "MemCache.cache.totalCost:" << cache.totalCost();
}

TileWrap::TileWrap(const QByteArray& a, const TileId &id)
{
    data = new QByteArray(a);
    tile = id;
    count++;
    total+=a.size();
    qDebug() << "MemCache tile inserted:" << tile << "(size:" << data->size() << "count:" << count << "totalsize:" << total << "wrapper:" << this << ")";
}

TileWrap::~TileWrap()
{
    count--;
    total-=data->size();
    qDebug() << "MemCache tile evicted" << tile << "(size:" << data->size() << "count:" << count << "totalsize:" << total << "wrapper:" << this << ")";
    delete data;
}

int TileWrap::count = 0;
int TileWrap::total = 0;
