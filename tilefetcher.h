#ifndef TILEFETCHER_H
#define TILEFETCHER_H

#include <QByteArray>
#include <QMultiMap>
#include <QObject>
#include <QThread>
//#include <QMutex>
#include <QList>
#include <QHash>
#include <QSet>

#include "tile.h"
#include "task.h"
#include "memcache.h"

class TileFetcher : public QObject
{
    Q_OBJECT
    public:
        explicit TileFetcher(QObject *parent = 0);
        ~TileFetcher();

        void wakeUp();

    signals:
        void tileData(const QString& type, int x, int y, int z,
                           const QByteArray& data);

    public slots:
        void fetchTile(const QString& type, int x, int y, int zoom);
        void forgetRequest(const QString& type, int x, int y, int zoom);

    private slots:
        void diskTileData(const QString& type, int x, int y, int z,
                          const QByteArray& data);
        void diskTaskFinished(Task* task);
        void networkTileData(const QString& type, int x, int y, int z,
                          const QByteArray& data);
        void networkTaskFinished(Task* task);

    protected:
        void customEvent(QEvent *);

    private:
        QEvent::Type wakeUpEvent;
        bool pendingWakeUp;

        void work();
        void debug(const QString& header);

        MemCache memCache;

        QSet<TileId> requests;
        QSet<TileId> diskRequests;
        QSet<TileId> networkRequests;
        QSet<TileId> activeNetworkRequests;

        QHash<TileId,QByteArray> diskWriteRequests;

        QSet<QThread*> idleDiskThreads;
        QSet<QThread*> activeDiskThreads;
        QSet<QThread*> idleNetworkThreads;
        QSet<QThread*> activeNetworkThreads;

        //QMutex mutex;
};

#endif // TILEFETCHER_H
