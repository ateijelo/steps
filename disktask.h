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
