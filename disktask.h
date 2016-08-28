/*
 * disktask.h -- part of Steps, a simple maps app
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

#ifndef DISKTASK_H
#define DISKTASK_H

#include <QByteArray>
#include "task.h"
#include "tile.h"

class DiskTask : public Task
{
    Q_OBJECT
    public:
        explicit DiskTask(QObject *parent = 0);
        void fetchTile(const QString& type, int x, int y, int zoom);
        void storeTile(const TileId& tile, QByteArray data);
        void storeTile(const QString& type, int x, int y, int zoom, QByteArray data);

    signals:
        void tileData(const QString& type, int x, int y, int zoom,
                      const QByteArray& data);

    public slots:

    protected:
        void work();

    private:
        QString getTileFileName(QString type, int x, int y, int zoom);
        QByteArray readMgm();
        QByteArray readSingleFile();

        bool store;
        QByteArray store_data;
        QString tile_type;
        int tile_x;
        int tile_y;
        int tile_zoom;
};

#endif // DISKTASK_H
