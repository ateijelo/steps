#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QObject>
#include <QRect>
#include <QByteArray>
#include <QLinkedList>

#include "tile.h"
#include "tilefetcher.h"

class TileLayer : public QObject
{
    Q_OBJECT

    typedef QLinkedList<Tile*> Column;
    typedef Column::iterator TilePointer;
    typedef QLinkedList<Column*>::iterator ColumnPointer;

    public:
        TileLayer();
        void setRegion(const QRect& r, int zoom);
        void clear();

    signals:
        void tileCreated(Tile *t,int x, int y, int zoom);

    private slots:
        void tileDataReady(const QByteArray& bytes);

    private:
        ColumnPointer adjustBeforeIntersection(const QRect& n, int zoom);
        void adjustColumn(Column* col, const QRect& n, int x, int zoom);
        void adjustAfterIntersection(const QRect& n, int zoom);
        void deleteColumn(Column* col);
        Tile* newTile(int x, int y, int zoom);

        QLinkedList<Column*> columns;
        QRect region;
        TileFetcher fetcher;
};

#endif // TILEMANAGER_H
