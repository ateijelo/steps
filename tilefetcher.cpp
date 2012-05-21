#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QByteArray>
#include <QApplication>

#include "disktask.h"
#include "networktask.h"
#include "tilefetcher.h"

TileFetcher::TileFetcher(QObject *parent) :
    QObject(parent)
{
    for (int i=0; i<1; i++)
    {
        QThread *t = new QThread(this);
        idleDiskThreads.insert(t);
        t->start();
    }
    for (int i=0; i<4; i++)
    {
        QThread *t = new QThread(this);
        idleNetworkThreads.insert(t);
        t->start();
    }

    wakeUpEvent = QEvent::Type(QEvent::registerEventType());
    pendingWakeUp = false;
}

TileFetcher::~TileFetcher()
{
    foreach (QThread *t, idleDiskThreads+activeDiskThreads+idleNetworkThreads+activeNetworkThreads)
    {
        t->quit();
        t->wait();
    }
}

void TileFetcher::customEvent(QEvent *event)
{
    if (event->type() == wakeUpEvent)
    {
        pendingWakeUp = false;
        work();
    }
}

void TileFetcher::wakeUp()
{
    qDebug() << "TileFetcher::wakeUp()";
    if (!pendingWakeUp)
    {
        pendingWakeUp = true;
        qApp->postEvent(this, new QEvent(wakeUpEvent));
    }
}

void TileFetcher::fetchTile(const QString &maptype, int x, int y, int zoom)
{
    qDebug() << "TileFetcher::fetchTile" << maptype << x << y << zoom;

//    QMutexLocker l(&mutex);
    TileId r(maptype,x,y,zoom);
    if (requests.contains(r))
    {
        qDebug() << "  request already queued.";
        return;
    }
    if (diskRequests.contains(r))
    {
        qDebug() << "  request already in disk queue.";
        return;
    }
    if (networkRequests.contains(r))
    {
        qDebug() << "  request already in network queue.";
    }

    requests.insert(r);
    wakeUp();
}

void TileFetcher::forgetRequest(const QString &type, int x, int y, int zoom)
{
    qDebug() << "TileFetcher::forgetRequest" << type << x << y << zoom;

//    QMutexLocker l(&mutex);
    TileId r(type,x,y,zoom);
    QSet<TileId>::iterator i;

    i = requests.find(r);
    if (i != requests.end())
    {
        requests.erase(i);
    }

    i = diskRequests.find(r);
    if (i != diskRequests.end())
    {
        diskRequests.erase(i);
    }

    i = networkRequests.find(r);
    if (i != networkRequests.end())
    {
        networkRequests.erase(i);
    }
}

void TileFetcher::diskTileData(const QString &type, int x, int y, int z,
                               const QByteArray &data)
{
    TileId tile(type,x,y,z);
    if (!data.isEmpty())
    {
        emit tileData(type,x,y,z,data);
        memCache.insert(tile,data);
        diskRequests.remove(tile);
    }
    else
    {
//        mutex.lock();
        networkRequests.insert(tile);
//        mutex.unlock();
    }
    wakeUp();
}

void TileFetcher::networkTileData(const QString &type, int x, int y, int z,
                                  const QByteArray &data)
{
    TileId tile(type,x,y,z);
    if (!data.isEmpty())
    {
        emit tileData(type,x,y,z,data);
        memCache.insert(tile,data);
        diskWriteRequests.insert(tile,data);
    }
    activeNetworkRequests.remove(tile);
    wakeUp();
}

void TileFetcher::diskTaskFinished(Task *task)
{
//    QThread* thread = task->thread();
//    QSet<QThread*>::iterator i = activeDiskThreads.find(thread);
//    if (i == activeDiskThreads.end())
//    {
//        qDebug("The thread of the disk task that just finished is not in the active list!");
//        exit(EXIT_FAILURE);
//    }
//    activeDiskThreads.erase(i);
//    idleDiskThreads.insert(thread);

    task->deleteLater();
    wakeUp();
}

void TileFetcher::networkTaskFinished(Task *task)
{
    QThread* thread = task->thread();
    QSet<QThread*>::iterator i = activeNetworkThreads.find(thread);
    if (i == activeNetworkThreads.end())
    {
        qDebug("The thread of the network task just finished is not in the active list!");
        exit(EXIT_FAILURE);
    }
    activeNetworkThreads.erase(i);
    idleNetworkThreads.insert(thread);

    NetworkTask *n = static_cast<NetworkTask*>(task);
    activeNetworkRequests.remove(n->tileId());

    task->deleteLater();
    wakeUp();
}

void TileFetcher::work()
{
//    QMutexLocker l(&mutex);
    while (requests.count() > 0)
    {
        QSet<TileId>::iterator i = requests.begin();
        TileId r = *i;

        QByteArray a = memCache.getTileData(r);
        if (!a.isEmpty())
        {
            emit tileData(r.type,r.x,r.y,r.zoom,a);
            requests.erase(i);
            continue;
        }

        requests.erase(i);
        diskRequests.insert(r);
    }

    while (qMin(idleDiskThreads.count(),diskRequests.count()) > 0)
    {
        QSet<TileId>::iterator i = diskRequests.begin();
        TileId r = *i;
        DiskTask *task = new DiskTask();
        task->fetchTile(r.type,r.x,r.y,r.zoom);

        connect(task,SIGNAL(tileData(QString,int,int,int,QByteArray)),
                this,SLOT(diskTileData(QString,int,int,int,QByteArray)));
        connect(task,SIGNAL(finished(Task*)),this,SLOT(diskTaskFinished(Task*)));

//        QSet<QThread*>::iterator j = idleDiskThreads.begin();
//        QThread *thread = *j;
//        activeDiskThreads.insert(thread);
//        idleDiskThreads.erase(j);
//        task->moveToThread(thread);
        task->start();

        diskRequests.erase(i);
    }

    while (qMin(idleDiskThreads.count(),diskWriteRequests.count()) > 0)
    {
        QHash<TileId,QByteArray>::iterator i = diskWriteRequests.begin();
        TileId r = i.key();
        QByteArray a = i.value();
        DiskTask *task = new DiskTask();
        task->storeTile(r,a);

        connect(task,SIGNAL(finished(Task*)),this,SLOT(diskTaskFinished(Task*)));

//        QSet<QThread*>::iterator j = idleDiskThreads.begin();
//        QThread *thread = *j;
//        activeDiskThreads.insert(thread);
//        idleDiskThreads.erase(j);
//        task->moveToThread(thread);
        task->start();

        diskWriteRequests.erase(i);
    }

    while (qMin(idleNetworkThreads.count(),networkRequests.count()) > 0)
    {
        QSet<TileId>::iterator i = networkRequests.begin();
        TileId r = *i;
        NetworkTask *task = new NetworkTask(r);

        connect(task,SIGNAL(tileData(QString,int,int,int,QByteArray)),
                this,SLOT(networkTileData(QString,int,int,int,QByteArray)));
        connect(task,SIGNAL(finished(Task*)),
                this,SLOT(networkTaskFinished(Task*)));

        QSet<QThread*>::iterator j = idleNetworkThreads.begin();
        QThread *thread = *j;
        activeNetworkThreads.insert(thread);
        idleNetworkThreads.erase(j);
        task->moveToThread(thread);
        task->start();

        networkRequests.erase(i);
        activeNetworkRequests.insert(r);
    }

    debug("TileFetcher::work");
}

void TileFetcher::debug(const QString& header)
{
    qDebug() << header;
//    qDebug() << "  idleDiskThreads:" << idleDiskThreads;
    
    qDebug() << "  requests:";
    foreach (const TileId& r, requests)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
    
    qDebug() << "  diskRequests:";
    foreach (const TileId& r, diskRequests)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
    
    qDebug() << "  networkRequests:";
    foreach (const TileId& r, networkRequests)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;

    qDebug() << "  activeNetworkRequests:";
    foreach (const TileId& r, activeNetworkRequests)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
}
