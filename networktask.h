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
