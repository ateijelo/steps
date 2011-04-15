#ifndef FETCHTASK_H
#define FETCHTASK_H

#include <QObject>
#include <QString>
#include <QEvent>

#include <QNetworkAccessManager>

class FetchTask : public QObject
{
    Q_OBJECT
    public:
        explicit FetchTask(const QString& tile_type, int x, int y, int zoom,
                           QObject *parent = 0);
        ~FetchTask();
        void customEvent(QEvent *);
        void start();

    signals:
        void tileData(const QString& type, int x, int y, int zoom,
                      const QByteArray data);
        void finished(FetchTask*);

    public slots:

    private slots:
        void replyFinished(QNetworkReply *reply);

    private:
        void work();
        QString getTileFileName(QString mapType, int x, int y, int zoom);

        enum TaskState { Start };

        QString tile_type;
        int tile_x;
        int tile_y;
        int tile_zoom;

        QEvent::Type startEvent;
        TaskState state;
};

#endif // FETCHTASK_H
