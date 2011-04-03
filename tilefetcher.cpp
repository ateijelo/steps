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

void TileFetcher::fetchTile(const QString &maptype, int x, int y, int zoom)
{
    FetchTask *task = new FetchTask(maptype,x,y,zoom);
    task->moveToThread(fetchThread);
    task->start();
}
