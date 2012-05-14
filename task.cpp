#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QApplication>

#include "task.h"

Task::Task(QObject *parent) :
    QObject(parent)
{
    startEvent = QEvent::Type(QEvent::registerEventType());
}

Task::~Task()
{
}

void Task::customEvent(QEvent *event)
{
    qDebug() << "Task::customEvent() (thread: " << thread() << ")";
    if (event->type() == startEvent)
    {
        work();
    }
}

void Task::start()
{
    qDebug() << "Task::start() (thread: " << thread() << ")";
    qApp->postEvent(this,new QEvent(startEvent));
}

void Task::work()
{
    emit finished(this);
}
