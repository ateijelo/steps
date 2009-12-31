#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QObject>
#include <QRect>
#include <QLinkedList>

#include "tile.h"

class TileManager : public QObject
{
    typedef QLinkedList<Tile*> Column;
    typedef Column::iterator TilePointer;
    typedef QLinkedList<Column*>::iterator ColumnPointer;

    Q_OBJECT
    public:
        TileManager();
        void setRegion(const QRect& r, int zoom);

    private:
        ColumnPointer adjustBeforeIntersection(const QRect& n, int zoom);        
        void adjustColumn(Column* col, const QRect& n, int x, int zoom);
        void adjustAfterIntersection(const QRect& n, int zoom);
        void deleteColumn(Column* col);

        QLinkedList<Column*> columns;
        QRect region;
};

#endif // TILEMANAGER_H
