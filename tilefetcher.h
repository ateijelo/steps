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
        void tileData(const QString& type, int x, int y, int z,
                           const QByteArray& data);

    public slots:

    private slots:
        void tileFetched(const QString& type, int x, int y, int z,
                         const QByteArray& data);

    private:
        QThread *fetchThread;

};

#endif // TILEFETCHER_H
