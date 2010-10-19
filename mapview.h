#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QContextMenuEvent>
#include <QSignalMapper>

#include "tile.h"
#include "geotools.h"
#include "mainscene.h"
#include "tilepyramid.h"

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
        bool canZoomIn();
        bool canZoomOut();
        int zoomLevel();

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
        //void setAsCenter(QObject *newCenter);
        void showLatLonAsToolTip(bool);

    private slots:
        void mouseMovedOverScene(const QPointF& scenePos);
        void copyToClipboard(QString text);

    private:
        void updateTiles();
        void addMenuAction(QSignalMapper *signalMapper, QMenu *menu, QString text);

        MainScene *scene;
        GeoTools gt;
        int zoom;
        qreal angle;
        QPointF sceneAnchor;
        QPoint viewAnchor;
        TilePyramid *tp;
        bool showToolTip;

};

#endif
