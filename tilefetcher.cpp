#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>

#include "fetchtask.h"
#include "tilefetcher.h"
#include "tile.h"

int qHash(const TileRequest& key)
{
    return qHash(QString("%1:%2:%3:%4")
                 .arg(key.type)
                 .arg(key.x)
                 .arg(key.y)
                 .arg(key.zoom));
}

TileFetcher::TileFetcher(QObject *parent) :
    QObject(parent)
{
    for (int i=0; i<4; i++)
    {
        QThread *t = new QThread(this);
        idleThreads.insert(t);
        t->start();
    }
}

TileFetcher::~TileFetcher()
{
    foreach (QThread *t, idleThreads)
    {
        t->quit();
        t->wait();
    }
    foreach (QThread *t, activeThreads)
    {
        t->quit();
        t->wait();
    }
}

void TileFetcher::tileFetched(const QString &type, int x, int y, int z,
                              const QByteArray &data)
{
    //qDebug() << "TileFetcher::tileFetched" << type << x << y << z;
    emit tileData(type,x,y,z,data);
    TileRequest r(type,x,y,z);
    if (idleRequests.contains(r))
    {
        //qDebug() << "  Removing fetched tile from queue";
        RequestPointer p = idleRequests.value(r);
        idleRequestQueue.erase(p);
    }
    //debug("  state:");
}

void TileFetcher::forgetRequest(const QString &type, int x, int y, int zoom)
{
    qDebug() << "TileFetcher::forgetRequest" << type << x << y << zoom;
    TileRequest r(type,x,y,zoom);
    QHash<TileRequest,RequestPointer>::iterator i = idleRequests.find(r);
    if (i != idleRequests.end())
    {
        qDebug() << "  Removing deleted tile from queue";
        RequestPointer p = i.value();
        idleRequestQueue.erase(p);
        idleRequests.erase(i);
    }
    //debug("  state:");
}

void TileFetcher::taskFinished(FetchTask *task)
{
    //qDebug() << "TileFetcher::taskFinished" << task;
    QThread *thread = task->thread();
    if (!activeRequestReverseMap.contains(task))
    {
        qDebug() << "Active requests do not contain" << task;
        exit(EXIT_FAILURE);
    }
    TileRequest r = activeRequestReverseMap.take(task);
    activeRequests.remove(r);
    activeThreads.remove(thread);

    idleThreads.insert(thread);
    task->deleteLater();

    schedule();
}

void TileFetcher::schedule()
{
    while (qMin(idleThreads.count(),idleRequestQueue.count()) > 0)
    {
        RequestPointer i = idleRequestQueue.begin();
        TileRequest r = *i;

        FetchTask *task = new FetchTask(r.type,r.x,r.y,r.zoom);
        connect(task,SIGNAL(tileData(QString,int,int,int,QByteArray)),
                this,SLOT(tileFetched(QString,int,int,int,QByteArray)));
        connect(task,SIGNAL(finished(FetchTask*)),
                this,SLOT(taskFinished(FetchTask*)));

        QSet<QThread*>::iterator j = idleThreads.begin();
        QThread *thread = *j;

        task->moveToThread(thread);
        task->start();

        activeThreads.insert(thread);
        activeRequests.insert(r);
        activeRequestReverseMap.insert(task,r);

        idleRequests.remove(r);
        idleRequestQueue.erase(i);
        idleThreads.erase(j);
    }
    debug("TileFetcher::schedule");
}

void TileFetcher::fetchTile(const QString &maptype, int x, int y, int zoom)
{
    qDebug() << "TileFetcher::fetchTile" << maptype << x << y << zoom;
    TileRequest r(maptype,x,y,zoom);
    if (activeRequests.contains(r))
    {
        qDebug() << "  request already active.";
        return;
    }
    if (idleRequests.contains(r))
    {
        qDebug() << "  request already queued.";
        return;
    }

    RequestPointer p = idleRequestQueue.insert(zoom,r);
    idleRequests.insert(r,p);

    schedule();
}

void TileFetcher::debug(const QString& header)
{
    qDebug() << header;
    qDebug() << "  idleThreads:" << idleThreads;
    qDebug() << "  idleRequests:";
    foreach (const TileRequest& r, idleRequestQueue)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
    qDebug() << "  activeThreads:" << activeThreads;
    qDebug() << "  activeRequests:";
    foreach (const TileRequest& r, activeRequests)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
}
