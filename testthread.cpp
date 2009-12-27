#include "testthread.h"

TestThread::TestThread(QGraphicsPixmapItem *i)
        : i(i), n(0)
{
}

void TestThread::run()
{
    while (true)
    {
        sleep(1);
        n++;
        i->setPixmap(QPixmap(QString("%1.png").arg(n%4+1)));
    }
}

