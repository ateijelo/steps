#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

class Tile : public QGraphicsPixmapItem
{
    public:
        Tile(int x, int y, int zoom);
        ~Tile();
    private:
        int x,y,zoom;
};

#endif // TILE_H
