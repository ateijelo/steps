#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLinkedList>
#include <QPoint>

#include "ui_mainwindow.h"
#include "geotools.h"
#include "tilemanager.h"
#include "tile.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    private:
        Ui::MainWindow ui;
        QGraphicsScene *scene;
        QLinkedList<QLinkedList<Tile*>*> columns;
        QLinkedList<Tile*> tiles;
        GeoTools gt;
        TileManager tm;
        QPointF center;
        int zoom;
        qreal angle;

    private slots:
        void mapViewHadToPaint();
        void mapViewMouseMoved(const QPoint&);
        void displayNewTile(Tile *t, int x, int y, int zoom);
        void zoomIn();
        void zoomOut();
        void rotRight();
        void rotLeft();
        void setZoomLevel(int zoom);
};

#endif
