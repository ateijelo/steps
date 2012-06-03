#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QContextMenuEvent>
#include <QSignalMapper>

#include "tile.h"
#include "mainscene.h"
#include "tilelayer.h"

#include "ui_viewwidgets.h"

class MapView : public QGraphicsView
{
    Q_OBJECT
    public:
        MapView(QWidget *parent=NULL);
        ~MapView();

        bool viewportEvent(QEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void leaveEvent(QEvent *event);
        void wheelEvent(QWheelEvent *event);
        void contextMenuEvent (QContextMenuEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        bool canZoomIn();
        bool canZoomOut();
        int zoomLevel();
        int maxZoomLevel();

    signals:
        void canZoomIn(bool);
        void canZoomOut(bool);
        void zoomChanged(int);
        void mouseMoved(const QPointF& latlon);

    public slots:
        void zoomIn();
        void zoomOut();
        void rotRight();
        void rotLeft();
        void setZoomLevel(int zoom);
        void setCacheStyle(QString cacheStyle);
        void setAsCenter(QObject *newCenter);
        void showLatLonAsToolTip(bool);

    private slots:
        void mouseMovedOverScene(const QPointF& scenePos);
        void copyToClipboard(QString text);

    private:
        void updateTiles();
        void addMenuAction(QSignalMapper *signalMapper, QMenu *menu, QString text);

        MainScene *scene;
        int zoom;
        qreal angle;
        QPointF sceneAnchor;
        QPoint viewAnchor;
        TileLayer tlayer;
        bool showToolTip;
        Ui::ViewWidgets ui;
        QString coordsTemplate;
};

#endif
