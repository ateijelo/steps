/*
 * tile.h -- part of Steps, a simple maps app
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

#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

class TileId
{
    public:
        TileId()
        {
            type = QString();
            x = y = zoom = 0;
        }

        TileId(QString type, int x, int y, int zoom)
        {
            this->type = type;
            this->x = x;
            this->y = y;
            this->zoom = zoom;
        }

        QString type;
        int x;
        int y;
        int zoom;
        bool operator==(const TileId& other) const
        {
            return other.type == type &&
                   other.x == x && other.y == y && other.zoom == zoom;
        }
};

int qHash(const TileId& key);

QDebug& operator<<(QDebug& debug, const TileId& tile);

typedef QPair<int,int> TileCoords;

class Tile : public QGraphicsPixmapItem
{

    public:
        Tile(QString type, int x, int y, int zoom);
        ~Tile();
        TileCoords coords() const;

        QString tileType() const;
        int tileX() const;
        int tileY() const;
        int zoom() const;
        int currentZoom() const;
        bool isWithin(int x, int y, int z) const;

        QString tileId() const;
        static QString tileKey(QString type, int x, int y, int z);
        void loadPixmap(const QPixmap pixmap, int zoom);

    private:
        QString _type;
        int _x,_y,_zoom,_current_zoom;
};

#endif // TILE_H
