#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = NULL);

    private:
        Ui::MainWindow ui;
        QGraphicsScene *scene;
};

#endif
