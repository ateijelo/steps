#ifndef MEMCACHE_H
#define MEMCACHE_H

#include <QObject>
#include <QCache>

#include "tile.h"

class MemCache : public QObject
{
    Q_OBJECT
public:
    explicit MemCache(QObject *parent = 0);

    QByteArray getTileData(const QString& type, int x, int y, int zoom) const;
    QByteArray getTileData(const TileId& id) const;
    bool contains(const QString& type, int x, int y, int zoom) const;
    bool contains(const TileId& id) const;
    void insert(const QString& type, int x, int y, int zoom, QByteArray data);
    void insert(const TileId& id, QByteArray data);
    
signals:
    
public slots:
    QCache<TileId,QByteArray> cache;
    
};

#endif // MEMCACHE_H
