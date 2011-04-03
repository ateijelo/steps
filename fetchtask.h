#ifndef FETCHTASK_H
#define FETCHTASK_H

#include <QPixmap>
#include <QObject>
#include <QString>
#include <QEvent>

class FetchTask : public QObject
{
    Q_OBJECT
    public:
        explicit FetchTask(const QString& tile_type, int x, int y, int zoom,
                           QObject *parent = 0);
        void customEvent(QEvent *);
        void start();

    signals:

    public slots:

    private:
        void work();
        QPixmap loadTile(QString mapType, int x, int y, int zoom);
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
