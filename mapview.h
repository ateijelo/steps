#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QContextMenuEvent>
#include <QSignalMapper>

#include "tile.h"
#include "mainscene.h"
#include "tilelayer.h"
#include "worldwindow.h"

#include "ui_viewwidgets.h"

class MapView : public QGraphicsView
{
    Q_OBJECT
    public:
        MapView(QWidget *parent=NULL);
        ~MapView();

        bool viewportEvent(QEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void leaveEvent(QEvent *event);
        void wheelEvent(QWheelEvent *event);
        void contextMenuEvent (QContextMenuEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void resizeEvent(QResizeEvent *event);
        bool canZoomIn();
        bool canZoomOut();
        int zoomLevel();
        int maxZoomLevel();
        int minZoomLevel();
        void centerScene();
        void refresh();

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
        void setAsCenter(QObject *);
        void showLatLonAsToolTip(bool);

    private slots:
        void mouseMovedOverScene(const QPointF& scenePos);
        void copyToClipboard(QString text);
        void setZoomLevelCentered(int zoom);
        void setZoomSliderTooltip(int zoom);

    private:
        void updateTiles();
        void addMenuAction(QSignalMapper *signalMapper, QMenu *menu, QString text);

        MainScene *scene;
        int zoom;
        qreal angle;
        QPointF sceneAnchor;
        QPoint viewAnchor;
        TileLayer tileLayer;
        bool showToolTip;
        Ui::ViewWidgets ui;
        QString coordsTemplate;
        QTransform lastViewportTransform;
        WorldWindow *worldWindow;
};

#endif
