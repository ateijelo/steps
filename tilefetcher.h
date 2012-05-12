#ifndef TILEFETCHER_H
#define TILEFETCHER_H

#include <QByteArray>
#include <QMultiMap>
#include <QObject>
#include <QThread>
#include <QList>
#include <QHash>
#include <QSet>

#include "fetchtask.h"
#include "tile.h"
#include "memcache.h"

class TileFetcher : public QObject
{
    Q_OBJECT
    public:
        explicit TileFetcher(QObject *parent = 0);
        ~TileFetcher();

        void fetchTile(const QString& type, int x, int y, int zoom);
        void forgetRequest(const QString& type, int x, int y, int zoom);

    signals:
        void tileData(const QString& type, int x, int y, int z,
                           const QByteArray& data);

    public slots:

    private slots:
        void tileFetched(const QString& type, int x, int y, int z,
                         const QByteArray& data);
        void taskFinished(FetchTask* task);

    protected:
        void customEvent(QEvent *);

    private:
        QEvent::Type wakeUpEvent;

        void work();
        void debug(const QString& header);

        typedef QMultiMap<int,TileId>::iterator RequestPointer;

        QSet<QThread*> idleThreads;

        // this hash is used to quickly locate a request in the queue
        // to avoid duplicates and to remove request on tileData.
        QHash<TileId,RequestPointer> idleRequests;

        // this is the requests queue, in ascending order of zoom level
        QMultiMap<int,TileId> idleRequestQueue;

        QSet<QThread*> activeThreads;
        QSet<TileId> activeRequests;

        // this map is to remove tasks from activeRequests after they finish
        QHash<const FetchTask*,TileId> activeRequestReverseMap;

        MemCache memCache;
};

#endif // TILEFETCHER_H
