#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QGraphicsPixmapItem>

class TestThread : public QThread
{
    public:
        TestThread(QGraphicsPixmapItem *i);
        void run();

    private:
        QGraphicsPixmapItem *i;
        int n;
};

#endif // TESTTHREAD_H
