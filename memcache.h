/*
 * memcache.h -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
