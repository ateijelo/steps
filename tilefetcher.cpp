#include <QApplication>
#include <QtAlgorithms>
#include <QByteArray>
#include <QSettings>
#include <QtEndian>
#include <QtDebug>
#include <QFile>
#include <QDir>

#include "disktask.h"
#include "networktask.h"
#include "tilefetcher.h"
#include "constants.h"
#include "debug.h"

TileFetcher::TileFetcher(QObject *parent) :
    QObject(parent)
{
    for (int i=0; i<2; i++)
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
    fDebug(DEBUG_FETCHREQUESTS) << "TileFetcher::customEvent()";
    if (event->type() == wakeUpEvent)
    {
        fDebug(DEBUG_FETCHREQUESTS) << "    wakeUpEvent";
        pendingWakeUp = false;
        work();
    }
}

void TileFetcher::wakeUp()
{
    fDebug(DEBUG_FETCHREQUESTS) << "TileFetcher::wakeUp()";
    if (!pendingWakeUp)
    {
        pendingWakeUp = true;
        qApp->postEvent(this, new QEvent(wakeUpEvent));
    }
}

void TileFetcher::fetchTile(const QString &maptype, int x, int y, int zoom)
{
    fDebug(DEBUG_FETCHREQUESTS) << "TileFetcher::fetchTile" << maptype << x << y << zoom;

//    QMutexLocker l(&mutex);
    TileId r(maptype,x,y,zoom);
    if (requests.contains(r))
    {
        fDebug(DEBUG_FETCHREQUESTS) << "  request already queued.";
        return;
    }
    if (diskRequests.contains(r))
    {
        fDebug(DEBUG_FETCHREQUESTS) << "  request already in disk queue.";
        return;
    }
    if (networkRequests.contains(r))
    {
        fDebug(DEBUG_FETCHREQUESTS) << "  request already in network queue.";
        return;
    }

    requests.insert(r);
    wakeUp();
}

void TileFetcher::forgetRequest(const QString &type, int x, int y, int zoom)
{
    fDebug(DEBUG_FETCHREQUESTS) << "TileFetcher::forgetRequest" << type << x << y << zoom;

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
    QThread* thread = task->thread();
    QSet<QThread*>::iterator i = activeDiskThreads.find(thread);
    if (i == activeDiskThreads.end())
    {
        fDebug(DEBUG_DISK) << "The thread of the disk task that just finished is not in the active list!";
        exit(EXIT_FAILURE);
    }
    activeDiskThreads.erase(i);
    idleDiskThreads.insert(thread);

    task->deleteLater();
    wakeUp();
}

void TileFetcher::networkTaskFinished(Task *task)
{
    QThread* thread = task->thread();
    QSet<QThread*>::iterator i = activeNetworkThreads.find(thread);
    if (i == activeNetworkThreads.end())
    {
        fDebug(DEBUG_NETWORK) << "The thread of the network task just finished is not in the active list!";
        exit(EXIT_FAILURE);
    }
    activeNetworkThreads.erase(i);
    idleNetworkThreads.insert(thread);

    NetworkTask *n = static_cast<NetworkTask*>(task);
    activeNetworkRequests.remove(n->tileId());

    task->deleteLater();
    wakeUp();
}

void TileFetcher::readMgm(const TileId& tile)
{
    int mgm_x = tile.x >> 3;
    int mgm_y = tile.y >> 3;
    QSettings settings;
    QString filename = QString("%1/%2_%3/%4_%5.mgm")
            .arg(settings.value(SettingsKeys::CachePath,"").toString())
            .arg(tile.type)
            .arg(tile.zoom)
            .arg(mgm_x)
            .arg(mgm_y);
    fDebug(DEBUG_DISK) << this << "started. Fetching" << tile.x << tile.y << "from" << filename;
    QFile mgm(filename);
    QHash<TileId,QPair<quint32,quint32> > mgmTiles;
    if (mgm.open(QIODevice::ReadOnly))
    {
        quint64 r = 0;
        quint32 tile_start = 64*6 + 2;
        quint32 tile_end;
        quint16 no_tiles;
        r += mgm.read((char*)(&no_tiles),2);
        if (r != 2)
        {
            fDebug(DEBUG_DISK) << "error reading no_tiles";
            no_tiles = 0;
        }
        no_tiles = qFromBigEndian(no_tiles);
        for (int i=0; i<no_tiles; i++)
        {
            quint8 tx,ty;
            r = mgm.read((char*)(&tx),1);
            r += mgm.read((char*)(&ty),1);
            r += mgm.read((char*)(&tile_end),4);
            if (r != 6)
            {
                fDebug(DEBUG_DISK) << "error reading tile entry " << i;
                break;
            }
            tile_end = qFromBigEndian(tile_end);
            TileId t(tile.type,tx + (mgm_x << 3),ty + (mgm_y << 3),tile.zoom);
            if (diskRequests.contains(t))
            {
                mgmTiles.insert(t,qMakePair(tile_start,tile_end - tile_start));
                diskRequests.remove(t);
            }
            tile_start = tile_end;
        }
    }
    if (!mgmTiles.contains(tile))
    {
        diskRequests.remove(tile);
        networkRequests.insert(tile);
    }
//    QSet<TileId>::iterator i = diskRequests.begin();
//    while (i != diskRequests.end())
//    {
//        TileId t = *i;
//        bool tBelongsHere = ((t.type == tile.type) &&
//                             ((t.x >> 3) == mgm_x) &&
//                             ((t.y >> 3) == mgm_y) &&
//                             (t.zoom == tile.zoom));
//        if (tBelongsHere)
//        {
//            fDebug(DEBUG_DISK) << "tile" << t << "in queue, in the range of the mgm, but absent.";
//            i = diskRequests.erase(i);
//            networkRequests.insert(t);
//        }
//        else
//        {
//            i++;
//        }
//    }
    for (QHash<TileId,QPair<quint32,quint32> >::iterator i = mgmTiles.begin(); i != mgmTiles.end(); i++)
    {
        TileId t = i.key();
        QPair<quint32,quint32> p = i.value();
        quint32 tile_start = p.first;
        quint32 tile_size = p.second;
        mgm.seek(tile_start);
        QByteArray data = mgm.read(tile_size);
        if (static_cast<quint32>(data.size()) != tile_size)
        {
            fDebug(DEBUG_DISK) << "error reading tile " << tile.x << "," << tile.y << "data";
            networkRequests.insert(t);
            continue;
        }
        else
        {
            fDebug(DEBUG_DISK) << "found tile" << t << "in mgm";
            emit tileData(t.type,t.x,t.y,t.zoom,data);
            memCache.insert(t,data);
        }
    }
}

bool TileFetcher::readSingleFile(const TileId& tile)
{
    QSettings settings;
    QDir d(settings.value(SettingsKeys::CachePath,"").toString());
    QFile f(d.absoluteFilePath(QString("cache/%1/%2/%3_%4").arg(tile.type).arg(tile.zoom).arg(tile.x).arg(tile.y)));
    fDebug(DEBUG_DISK) << "opening" << f.fileName();
    if (f.open(QIODevice::ReadOnly))
    {
        QByteArray data = f.readAll();
        emit tileData(tile.type,tile.x,tile.y,tile.zoom,data);
        memCache.insert(tile,data);
        diskRequests.remove(tile);
        return true;
    }
    fDebug(DEBUG_DISK) << "   failed";
    return false;
}

void TileFetcher::work()
{
    debug("TileFetcher::work");
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

    while (diskRequests.count() > 0)
    {
        debug("while (diskRequests.count() > 0):");
        QSet<TileId>::iterator i = diskRequests.begin();
        TileId tile = *i;

        if (readSingleFile(tile))
            continue;
        readMgm(tile);
    }

    while (qMin(idleDiskThreads.count(),diskWriteRequests.count()) > 0)
    {
        QHash<TileId,QByteArray>::iterator i = diskWriteRequests.begin();
        TileId r = i.key();
        QByteArray a = i.value();
        DiskTask *task = new DiskTask();
        task->storeTile(r,a);

        connect(task,SIGNAL(finished(Task*)),this,SLOT(diskTaskFinished(Task*)));

        QSet<QThread*>::iterator j = idleDiskThreads.begin();
        QThread *thread = *j;
        activeDiskThreads.insert(thread);
        idleDiskThreads.erase(j);
        task->moveToThread(thread);
        task->start();

        diskWriteRequests.erase(i);
    }

    while (qMin(idleNetworkThreads.count(),networkRequests.count()) > 0)
    {
        QList<TileId> l = networkRequests.values();
        qSort(l.begin(),l.end(),fetchOrder);

        QSet<TileId>::iterator i = networkRequests.find(l.at(0));
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

}

bool fetchOrder(const TileId& t1, const TileId& t2)
{
    if (t1.zoom < t2.zoom)
        return true;
    if (t1.zoom > t2.zoom)
        return false;
    if (t1.y < t2.y)
        return true;
    if (t1.y > t2.y)
        return false;
    if (t1.x < t2.x)
        return true;
    if (t1.x > t2.x)
        return false;
    return false;
}

void TileFetcher::debug(const QString& header)
{
    if fEnabled(DEBUG_FETCHQUEUES)
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
}
