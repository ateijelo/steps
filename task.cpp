/*
 * task.cpp -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
