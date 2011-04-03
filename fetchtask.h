#ifndef FETCHTASK_H
#define FETCHTASK_H

#include <QObject>
#include <QString>
#include <QEvent>

class FetchTask : public QObject
{
    Q_OBJECT
    public:
        explicit FetchTask(const QString& maptype, int x, int y, int zoom,
                           QObject *parent = 0);
        void customEvent(QEvent *);
        void start();

    signals:

    public slots:

    private:
        QString maptype;
        int tile_x;
        int tile_y;
        int tile_zoom;

        QEvent::Type startEvent;
        enum TaskState { START } state;
};

#endif // FETCHTASK_H
