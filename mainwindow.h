#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QList>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    private:
        Ui::MainWindow ui;
        QGraphicsScene *scene;
        QList<QGraphicsPixmapItem*> tiles;

private slots:
    void on_pushButton_clicked();
};

#endif
