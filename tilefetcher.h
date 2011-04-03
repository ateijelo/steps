#ifndef TILEFETCHER_H
#define TILEFETCHER_H

#include <QObject>

class TileFetcher : public QObject
{
    Q_OBJECT
    public:
        explicit TileFetcher(QObject *parent = 0);

    signals:

    public slots:

};

#endif // TILEFETCHER_H
