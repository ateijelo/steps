/*
 * tilelayer.h -- part of Steps, a simple maps app
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

#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QHash>
#include <QRect>
#include <QObject>
#include <QPoint>
#include <QThread>
#include <QByteArray>
#include <QLinkedList>
#include <QGraphicsScene>

#include "tile.h"
#include "tilefetcher.h"
#include "debug.h"

class TileLayer : public QObject
{
    Q_OBJECT

    typedef QLinkedList<Tile*> Column;
    typedef Column::iterator TilePointer;
    typedef QLinkedList<Column*>::iterator ColumnPointer;

    public:
        TileLayer(QGraphicsScene *scene);
        void setRegion(const QRect& r, int zoom);
        void clear();
        void reload();

    signals:
        void fetchTile(const QString& type, int x, int y, int z);
        void forgetTile(const QString& type, int x, int y, int z);
        void reloading();
        void loadedMBTiles(const QString& path);

    private slots:
        void tileData(const QString& type, int x, int y, int z,
                           const QByteArray& bytes);

    private:
        ColumnPointer adjustBeforeIntersection(const QRect& n);
        void adjustColumn(Column* col, const QRect& n, int x);
        void adjustAfterIntersection(const QRect& n);
        void deleteColumn(Column* col);
        Tile* newTile(int x, int y);
        void deleteTile(Tile *t);
        int modx(int x);
        void setTilesInRegion(const QRect& i, const QPixmap p, int z);

        int zoom;
        QString type;
        QString tileKeyTemplate;
        QRect region;
        TileFetcher fetcher;
        QThread* fetcherThread;
        QLinkedList<Column*> columns;
        QHash<TileCoords,Tile*> tiles; // TileCoords is defined in tile.h
        QSet<TileId> fetchRequests;
        QGraphicsScene *scene;

};

#endif // TILEMANAGER_H
