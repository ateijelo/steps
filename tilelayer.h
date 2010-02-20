#ifndef TILELAYER_H
#define TILELAYER_H

#include <QGraphicsScene>
#include <QLinkedList>
#include <QRectF>

#include "tile.h"
#include "geotools.h"

class TileLayer
{
    typedef QLinkedList<Tile*> Column;
    typedef Column::iterator TilePointer;
    typedef QLinkedList<Column*>::iterator ColumnPointer;

    public:
        TileLayer(int zoom, QGraphicsScene *scene);
        void setRegion(const QRectF& r);
        void clear();

    private:
        ColumnPointer adjustBeforeIntersection(const QRect& n);
        void adjustColumn(Column* col, const QRect& n, int x);
        void adjustAfterIntersection(const QRect& n);
        void deleteColumn(Column* col);
        Tile *newTile(int x, int y);

        int zoom;
        QGraphicsScene *scene;
        QRectF sceneRegion;
        QLinkedList<Column*> columns;
        QRect tileRegion;
        GeoTools gt;
        QRectF innerSafeRect;
        QRectF outerSafeRect;
//        TileLayer();
//        void setRegion(const QRect& r, int zoom);
//        void clear();
//
//    signals:
//        void tileCreated(Tile *t,int x, int y, int zoom);
//
//    private:
//
//        QLinkedList<Column*> columns;
//        QRect region;
};

#endif // TILELAYER_H
