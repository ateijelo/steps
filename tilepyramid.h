#ifndef TILEPYRAMID_H
#define TILEPYRAMID_H

#include <QGraphicsScene>
#include <QRectF>

#include "tilelayer.h"
#include "geotools.h"

class TilePyramid
{
    public:
        TilePyramid(QGraphicsScene *scene);
        void setRegion(const QRectF& sceneRegion);
        void displayUpToLevel(int z);

    private:
        QGraphicsScene *scene;
        QRectF sceneRegion;
        QList<TileLayer*> layers;
        GeoTools gt;
};

#endif // TILEPYRAMID_H
