#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

#define TILE_STYLE_MAP "GoogleMap"
#define TILE_STYLE_SAT "GoogleSat"
#define TILE_STYLE_HYB "GoogleHyb"

class Tile : public QGraphicsPixmapItem
{
    public:
        Tile(int x, int y, int zoom);
        ~Tile();
    private:
        QString getTileFileName(QString tileStyle, int x, int y, int zoom);
        QPixmap loadTile(QString tileStyle, int x, int y, int zoom);

    private:
        int x,y,zoom;
};

#endif // TILE_H
