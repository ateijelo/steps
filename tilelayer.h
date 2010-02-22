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
        bool wouldChangeWithRegion(const QRectF& r);
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
        QLinkedList<Column*> columns;
        QRect tileRegion;
        GeoTools gt;

        QRectF topLeftSafeRange;
        QRectF bottomRightSafeRange;
};

#endif // TILELAYER_H
