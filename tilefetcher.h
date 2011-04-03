#ifndef TILEFETCHER_H
#define TILEFETCHER_H

#include <QObject>
#include <QByteArray>
#include <QThread>

class TileFetcher : public QObject
{
    Q_OBJECT
    public:
        explicit TileFetcher(QObject *parent = 0);
        ~TileFetcher();

        void fetchTile(const QString& maptype, int x, int y, int zoom);

    signals:
        void tileDataReady(const QByteArray&);

    public slots:

    private:
        QThread *fetchThread;

};

#endif // TILEFETCHER_H
