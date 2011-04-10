#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

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
        void loadPixmap(const QPixmap &pixmap, int zoom);

    private:
        QString _type;
        int _x,_y,_zoom,_current_zoom;
};

#endif // TILE_H
