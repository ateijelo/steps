#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QApplication>

#include "fetchtask.h"
#include "tilefetcher.h"

TileFetcher::TileFetcher(QObject *parent) :
    QObject(parent)
{
    for (int i=0; i<4; i++)
    {
        QThread *t = new QThread(this);
        idleThreads.insert(t);
        t->start();
    }

    wakeUpEvent = QEvent::Type(QEvent::registerEventType());
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

void TileFetcher::customEvent(QEvent *event)
{
    if (event->type() == wakeUpEvent)
    {
        work();
    }
}

void TileFetcher::wakeUp()
{
    qApp->postEvent(this, new QEvent(wakeUpEvent));
}

void TileFetcher::fetchTile(const QString &maptype, int x, int y, int zoom)
{
    qDebug() << "TileFetcher::fetchTile" << maptype << x << y << zoom;
    TileId r(maptype,x,y,zoom);
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

    wakeUp();
}

void TileFetcher::tileFetched(const QString &type, int x, int y, int z,
                              const QByteArray &data)
{

}

void TileFetcher::forgetRequest(const QString &type, int x, int y, int zoom)
{
    qDebug() << "TileFetcher::forgetRequest" << type << x << y << zoom;
    TileId r(type,x,y,zoom);
    QHash<TileId,RequestPointer>::iterator i = idleRequests.find(r);
    if (i != idleRequests.end())
    {
        qDebug() << "  Removing deleted tile from queue";
        RequestPointer p = i.value();
        idleRequestQueue.erase(p);
        idleRequests.erase(i);
    }
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
    TileId r = activeRequestReverseMap.take(task);
    activeRequests.remove(r);
    activeThreads.remove(thread);

    idleThreads.insert(thread);
    task->deleteLater();

    work();
}

void TileFetcher::work()
{
    while (idleRequestQueue.count()) > 0)
    {
        RequestPointer i = idleRequestQueue.begin();
        TileId r = *i;

        QByteArray a = memCache.getTileData(r);
        if (!a.isEmpty())
        {
            emit tileData(r.type,r.x,r.y,r.zoom,a);
            idleR
        }


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

void TileFetcher::debug(const QString& header)
{
    qDebug() << header;
    qDebug() << "  idleThreads:" << idleThreads;
    qDebug() << "  idleRequests:";
    foreach (const TileId& r, idleRequestQueue)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
    qDebug() << "  activeThreads:" << activeThreads;
    qDebug() << "  activeRequests:";
    foreach (const TileId& r, activeRequests)
        qDebug() << "   " << r.type << r.x << r.y << r.zoom;
}
