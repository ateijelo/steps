#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

typedef QPair<int,int> TileCoords;

class Tile : public QGraphicsPixmapItem
{

    public:
        Tile(int x, int y, int zoom);
        ~Tile();
        TileCoords coords();

    private:

    private:
        int x,y,zoom;
};

#endif // TILE_H
