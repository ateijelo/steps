#ifndef TILEPYRAMID_H
#define TILEPYRAMID_H

#include <QGraphicsItem>
#include <QRectF>

#include "tilelayer.h"

class TilePyramid : public QGraphicsItem
{
    public:
        TilePyramid(QGraphicsItem *parent=0);

    private:
        QRectF region;
        QList<TileLayer*> layers;
};

#endif // TILEPYRAMID_H
