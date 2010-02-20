#ifndef TILEPYRAMID_H
#define TILEPYRAMID_H

#include <QGraphicsItem>
#include <QRectF>

#include "tilelayer.h"
#include "geotools.h"

class TilePyramid : public QGraphicsItem
{
    public:
        TilePyramid(QGraphicsItem *parent=0);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
        void setRegion(const QRectF& sceneRegion);
        void displayUpToLevel(int z);

    private:
        QRectF sceneRegion;
        QList<TileLayer*> layers;
        GeoTools gt;
};

#endif // TILEPYRAMID_H
