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

    private slots:
        void replyFinished(QNetworkReply *reply);

    protected:
        void work();

    private:
        TileId tile;
        QNetworkAccessManager *net;
    
};

#endif // NETWORKTASK_H
