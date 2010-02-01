#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

#define TILE_STYLE_MAP 0
#define TILE_STYLE_SAT 1
#define TILE_STYLE_HYB 2

class Tile : public QGraphicsPixmapItem
{
    public:
        Tile(int tileStyle, int x, int y, int zoom);
        ~Tile();
    private:
        QString getMapTileFileName(int x, int y, int zoom);
        QString getSatTileFileName(int x, int y, int zoom);
        QString getHybTileFileName(int x, int y, int zoom);
        QString getTileFileName(QString filenameFormat, int x, int y, int zoom);
        QPixmap getProperTile(QString filename, int x, int y);

    private:
        int x,y,zoom;
};

#endif // TILE_H
