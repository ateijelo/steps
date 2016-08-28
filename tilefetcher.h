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
#include <QtSql/QSqlDatabase>

#include "tile.h"
#include "task.h"
#include "memcache.h"
#include "networktask.h"

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
        void loadedMBTiles(const QString& path);

    public slots:
        void fetchTile(const QString& type, int x, int y, int zoom);
        void forgetRequest(const QString& type, int x, int y, int zoom);
        void reload();

    private slots:
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
        void readMgm(const TileId &tile);
        QByteArray readMBTile(const TileId &tile);
        bool readSingleFile(const TileId &tile);

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

        QSqlDatabase db;

        //QMutex mutex;
};

bool fetchOrder(const TileId& t1, const TileId& t2);

#endif // TILEFETCHER_H
