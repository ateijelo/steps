/*
 * task.h -- part of Steps, a simple maps app
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

#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QEvent>

class Task : public QObject
{
    Q_OBJECT
    public:
        explicit Task(QObject *parent = 0);
        ~Task();

        void customEvent(QEvent *);
        void start();

    signals:
        void finished(Task*);

    public slots:

    protected:
        virtual void work();

    QEvent::Type startEvent;
};

#endif // TASK_H
