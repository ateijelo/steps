#include "fetchtask.h"
#include "tilefetcher.h"

TileFetcher::TileFetcher(QObject *parent) :
    QObject(parent)
{
    fetchThread = new QThread(this);
    fetchThread->start();
}

TileFetcher::~TileFetcher()
{
    fetchThread->quit();
    fetchThread->wait();
}

void TileFetcher::tileFetched(const QString &type, int x, int y, int z,
                              const QByteArray &data)
{
    emit tileData(type,x,y,z,data);
}

void TileFetcher::fetchTile(const QString &maptype, int x, int y, int zoom)
{
    FetchTask *task = new FetchTask(maptype,x,y,zoom);
    task->moveToThread(fetchThread);
    connect(task,SIGNAL(tileData(QString,int,int,int,QByteArray)),
            this,SLOT(tileFetched(QString,int,int,int,QByteArray)));
    task->start();
}
