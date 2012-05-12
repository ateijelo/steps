#include "memcache.h"

MemCache::MemCache(QObject *parent) :
    QObject(parent)
{
    cache.setMaxCost(100*1024);
}

QByteArray MemCache::getTileData(const QString &type, int x, int y, int zoom) const
{
    TileId t(type,x,y,zoom);
    return getTileData(t);
}

QByteArray MemCache::getTileData(const TileId &id) const
{
    QByteArray *a = cache.object(id);
    if (a == 0)
        return QByteArray();
    return *a;
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

void MemCache::insert(const QString &type, int x, int y, int zoom, QByteArray data)
{
    TileId t(type,x,y,zoom);
    insert(t,data);
}

void MemCache::insert(const TileId &id, QByteArray data)
{
    cache.insert(id,&data,data.size());
}
