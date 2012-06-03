#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QHash>
#include <QRect>
#include <QObject>
#include <QPoint>
#include <QThread>
#include <QByteArray>
#include <QLinkedList>
#include <QGraphicsScene>

#include "tile.h"
#include "tilefetcher.h"
#include "debug.h"

class TileLayer : public QObject
{
    Q_OBJECT

    typedef QLinkedList<Tile*> Column;
    typedef Column::iterator TilePointer;
    typedef QLinkedList<Column*>::iterator ColumnPointer;

    public:
        TileLayer(QGraphicsScene *scene);
        void setRegion(const QRect& r, int zoom);
        void clear();

    signals:
        void fetchTile(const QString& type, int x, int y, int z);
        void forgetTile(const QString& type, int x, int y, int z);

    private slots:
        void tileData(const QString& type, int x, int y, int z,
                           const QByteArray& bytes);

    private:
        ColumnPointer adjustBeforeIntersection(const QRect& n);
        void adjustColumn(Column* col, const QRect& n, int x);
        void adjustAfterIntersection(const QRect& n);
        void deleteColumn(Column* col);
        Tile* newTile(int x, int y);
        void deleteTile(Tile *t);
        int modx(int x);
        void setTilesInRegion(const QRect& i, const QPixmap p, int z);

        int zoom;
        QString type;
        QString tileKeyTemplate;
        QRect region;
        TileFetcher fetcher;
        QThread* fetcherThread;
        QLinkedList<Column*> columns;
        QHash<TileCoords,Tile*> tiles; // TileCoords is defined in tile.h
        QSet<TileId> fetchRequests;
        QGraphicsScene *scene;

};

#endif // TILEMANAGER_H
