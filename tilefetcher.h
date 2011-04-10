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

class TileRequest
{
    public:
        TileRequest()
        {
            type = QString();
            x = y = zoom = 0;
        }

        TileRequest(QString type, int x, int y, int zoom)
        {
            this->type = type;
            this->x = x;
            this->y = y;
            this->zoom = zoom;
        }

        QString type;
        int x;
        int y;
        int zoom;
        bool operator==(const TileRequest& other) const
        {
            return other.type == type &&
                   other.x == x && other.y == y && other.zoom == zoom;
        }
};

int qHash(const TileRequest& key);

class TileFetcher : public QObject
{
    Q_OBJECT
    public:
        explicit TileFetcher(QObject *parent = 0);
        ~TileFetcher();

        void fetchTile(const QString& maptype, int x, int y, int zoom);

    signals:
        void tileData(const QString& type, int x, int y, int z,
                           const QByteArray& data);

    public slots:

    private slots:
        void tileFetched(const QString& type, int x, int y, int z,
                         const QByteArray& data);
        void taskFinished(FetchTask* task);

    private:
        void schedule();
        void debug(const QString& header);

        typedef QMultiMap<int,TileRequest>::iterator RequestPointer;

        QSet<QThread*> idleThreads;
        // this hash is used to quickly locate a request in the queue
        // to avoid duplicates and to remove request on tileData.
        QHash<TileRequest,RequestPointer> idleRequests;
        // this is the requests queue, in ascending order of zoom level
        QMultiMap<int,TileRequest> idleRequestQueue;

        QSet<QThread*> activeThreads;
        QSet<TileRequest> activeRequests;
        // this map is to remove tasks from activeRequests after they finish
        QHash<const FetchTask*,TileRequest> activeRequestReverseMap;
};

#endif // TILEFETCHER_H
