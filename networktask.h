/*
 * networktask.h -- part of Steps, a simple maps app
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

#ifndef NETWORKTASK_H
#define NETWORKTASK_H

#include "task.h"
#include "tile.h"

#include <QNetworkAccessManager>

class NetworkTask : public Task
{
    Q_OBJECT
    public:
        explicit NetworkTask(const TileId& tile, QObject *parent = 0);
        TileId tileId();
        ~NetworkTask();

    signals:
        void tileData(const QString& type, int x, int y, int zoom,
                      const QByteArray& data);

    public slots:
        void stop();

    private slots:
        void replyFinished(QNetworkReply *r);

    protected:
        void work();

    private:
        TileId tile;
        QNetworkAccessManager *net;
        QNetworkReply *reply;
    
};

#endif // NETWORKTASK_H
