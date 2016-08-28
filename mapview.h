/*
 * mapview.h -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#include "path.h"

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
        void reload();
        void addPath(Path *p);

    signals:
        void canZoomIn(bool);
        void canZoomOut(bool);
        void zoomChanged(int);
        void mouseMoved(const QPointF& latlon);
        void loadedMBTiles(const QString& path);

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
