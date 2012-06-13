#ifndef WORLDWINDOW_H
#define WORLDWINDOW_H

#include <QWidget>

class MapView;

class WorldWindow : public QWidget
{
    Q_OBJECT
    public:
        WorldWindow(MapView *parent = 0);
        void paintEvent(QPaintEvent *);

    private:
        QRectF _bRect;
        MapView *view;
};

#endif // WORLDWINDOW_H
