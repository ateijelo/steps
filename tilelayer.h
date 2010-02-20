#ifndef TILELAYER_H
#define TILELAYER_H

#include <QGraphicsItem>
#include <QLinkedList>
#include <QRectF>

#include "tile.h"
#include "geotools.h"

class TileLayer : public QGraphicsItem
{
    typedef QLinkedList<Tile*> Column;
    typedef Column::iterator TilePointer;
    typedef QLinkedList<Column*>::iterator ColumnPointer;

    public:
        TileLayer(int zoom, QGraphicsItem *parent=NULL);
        void setRegion(const QRectF& r);
        void clear();
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);

    private:
        ColumnPointer adjustBeforeIntersection(const QRect& n);
        void adjustColumn(Column* col, const QRect& n, int x);
        void adjustAfterIntersection(const QRect& n);
        void deleteColumn(Column* col);
        Tile *newTile(int x, int y);

        int zoom;
        QRectF sceneRegion;
        QLinkedList<Column*> columns;
        QRect tileRegion;
        GeoTools gt;
        QRectF innerSafeRect;
        QRectF outerSafeRect;
        bool singleTile;
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
