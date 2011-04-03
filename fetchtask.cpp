#include <QtDebug>
#include <QApplication>
#include <unistd.h>

#include "fetchtask.h"

FetchTask::FetchTask(const QString &maptype, int x, int y, int zoom,
                     QObject *parent) :
    QObject(parent)
{
    this->maptype = maptype;
    this->tile_x = x;
    this->tile_y = y;
    this->tile_zoom = zoom;

    startEvent = QEvent::Type(QEvent::registerEventType());
}

void FetchTask::customEvent(QEvent *event)
{
    if (event->type() == startEvent)
    {
        state = START;
        work();
    }
}

void FetchTask::start()
{
    qApp->postEvent(this,new QEvent(startEvent));
}
