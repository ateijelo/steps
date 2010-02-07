#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>

#include "tile.h"
#include "geotools.h"
#include "mainscene.h"
#include "tilemanager.h"

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

    signals:
        void canZoomIn(bool);
        void canZoomOut(bool);
        void zoomChanged(int);
        void mouseMoved(const QPointF& latlon);

    public slots:
        void displayNewTile(Tile *t, int x, int y, int zoom);
        void zoomIn();
        void zoomOut();
        void rotRight();
        void rotLeft();
        void setZoomLevel(int zoom);
        void setMapStyle();
        void setSatStyle();
        void setHybStyle();

    private slots:
        void mouseMovedOverScene(const QPointF& scenePos);

    private:
        void updateTiles();

        MainScene *scene;
        GeoTools gt;
        int zoom;
        qreal angle;
        QPointF sceneAnchor;
        QPoint viewAnchor;
        TileManager tm;
};

#endif
