#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLinkedList>
#include <QPoint>

#include "ui_mainwindow.h"
#include "geotools.h"
#include "tile.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    private:
        Ui::MainWindow ui;
        QGraphicsScene *scene;
        QLinkedList<Tile*> tiles;
        GeoTools gt;
        QPointF center;
        int zoom;

    private slots:
        void mapViewHadToPaint();
        void test();
};

#endif
