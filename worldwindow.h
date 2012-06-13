#ifndef WORLDWINDOW_H
#define WORLDWINDOW_H

#include <QWidget>

class WorldWindow : public QWidget
{
    Q_OBJECT
    public:
        WorldWindow(QWidget *parent = 0);
        void paintEvent(QPaintEvent *);
        void resize(const QSize& size);

    public slots:
        void zoomChanged(int z);

    private:
        void updateMask();
        int z;
};

#endif // WORLDWINDOW_H
