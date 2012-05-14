#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QEvent>

class Task : public QObject
{
    Q_OBJECT
    public:
        explicit Task(QObject *parent = 0);
        ~Task();

        void customEvent(QEvent *);
        void start();

    signals:
        void finished(Task*);

    public slots:

    protected:
        virtual void work();

    QEvent::Type startEvent;
};

#endif // TASK_H
