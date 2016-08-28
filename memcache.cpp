/*
 * memcache.cpp -- part of Steps, a simple maps app
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

#include "memcache.h"
#include "debug.h"

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
        fDebug(DEBUG_MEMCACHE) << "MemCache miss" << id;
        return QByteArray();
    }
    fDebug(DEBUG_MEMCACHE) << "MemCache hit" << id;
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
    fDebug(DEBUG_MEMCACHE) << "MemCache.cache.count:" << cache.count();
    fDebug(DEBUG_MEMCACHE) << "MemCache.cache.maxCost:" << cache.maxCost();
    fDebug(DEBUG_MEMCACHE) << "MemCache.cache.totalCost:" << cache.totalCost();
}

void MemCache::clear()
{
    cache.clear();
}

TileWrap::TileWrap(const QByteArray& a, const TileId &id)
{
    data = new QByteArray(a);
    tile = id;
    count++;
    total+=a.size();
    fDebug(DEBUG_MEMCACHE) << "MemCache tile inserted:" << tile << "(size:" << data->size() << "count:" << count << "totalsize:" << total << "wrapper:" << this << ")";
}

TileWrap::~TileWrap()
{
    count--;
    total-=data->size();
    fDebug(DEBUG_MEMCACHE) << "MemCache tile evicted" << tile << "(size:" << data->size() << "count:" << count << "totalsize:" << total << "wrapper:" << this << ")";
    delete data;
}

int TileWrap::count = 0;
int TileWrap::total = 0;
