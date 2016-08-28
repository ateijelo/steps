#ifndef MEMCACHE_H
#define MEMCACHE_H

#include <QObject>
#include <QCache>

#include "tile.h"

class TileWrap
{
public:
    TileWrap(const QByteArray &data, const TileId &id);
    ~TileWrap();
    static int count;
    static int total;
    QByteArray *data;
    TileId tile;
};

class MemCache : public QObject
{
    Q_OBJECT
    public:
        explicit MemCache(QObject *parent = 0);

        QByteArray getTileData(const QString& type, int x, int y, int zoom) const;
        QByteArray getTileData(const TileId& id) const;
        bool contains(const QString& type, int x, int y, int zoom) const;
        bool contains(const TileId& id) const;
        void insert(const QString& type, int x, int y, int zoom, const QByteArray &data);
        void insert(const TileId& id, const QByteArray &data);
        void clear();

    signals:

    public slots:

    private:
        QCache<TileId,TileWrap> cache;
    
};

#endif // MEMCACHE_H
