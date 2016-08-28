/*
 * mapview.cpp -- part of Steps, a simple maps app
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

#include <QtDebug>
#include <QSettings>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QToolTip>
#include <QHBoxLayout>
#include <QSlider>
#include <QPixmapCache>
#include <math.h>

#include "mapview.h"
#include "constants.h"
#include "path.h"
#include "geotools.h"
#include "geocircle.h"
#include "worldwindow.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent), scene(new MainScene()), tileLayer(scene)
{
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setScene(scene);
    setSceneRect(QRectF(GeoTools::Pixels2Meters(QPointF(INT_MIN,0),0),
                        GeoTools::Pixels2Meters(QPointF(INT_MAX,256),0)));

//#ifdef Q_OS_MAC
//    setViewportUpdateMode(FullViewportUpdate);
//#endif

    connect(scene,SIGNAL(mouseMoved(QPointF)),this,SLOT(mouseMovedOverScene(QPointF)));

    QSettings settings;

    angle = settings.value(SettingsKeys::Angle, 0.0F).toDouble();

    zoom = -1;
    setZoomLevel(settings.value(SettingsKeys::ZoomLevel, 0).toInt());

    qreal lat = settings.value(SettingsKeys::Latitude, 0).toDouble();
    qreal lon = settings.value(SettingsKeys::Longitude, 0).toDouble();
    centerOn(GeoTools::LatLon2Meters(QPointF(lon,lat)));

    showToolTip = settings.value(SettingsKeys::ShowLatLonAsToolTip, false).toBool();

    coordsTemplate = QString::fromUtf8(" %1°%2'%3\"%4 %5°%6'%7\"%8 ");


//    p = new PathGraphicsItem();
//    p->setZValue(2);
//    p->setPos(QPointF(GeoTools::projectionWidth(),0));
//    scene->addItem(p);

//    p = new PathGraphicsItem();
//    p->setZValue(2);
//    p->setPos(QPointF(-GeoTools::projectionWidth(),0));
//    scene->addItem(p);

//    GeoCircle *c = new GeoCircle();
//    c->setZValue(3);
//    c->setPos(GeoTools::LatLon2Meters(QPointF(-82.4,23)));
//    scene->addItem(c);

    //p = new PathGraphicsItem();
    //p->setZValue(100);
    //p->setPos(gt.LatLon2Meters(QPointF(-81.6884,23.1503)));
    //scene->addItem(p);

    worldWindow = new WorldWindow(this);
    ui.setupUi(this);
    connect(this,SIGNAL(zoomChanged(int)),worldWindow,SLOT(zoomChanged(int)));
    worldWindow->zoomChanged(zoom);
    ui.zoomSlider->setMinimum(0);
    ui.zoomSlider->setMaximum(maxZoomLevel());
    ui.zoomSlider->setValue(zoom);
    setZoomSliderTooltip(zoom);
    connect(ui.zoomSlider, &QSlider::valueChanged, this, &MapView::setZoomLevelCentered);
    connect(this, &MapView::zoomChanged, ui.zoomSlider, &QSlider::setValue);
    connect(this, &MapView::zoomChanged, this, &MapView::setZoomSliderTooltip);
    connect(&tileLayer, &TileLayer::loadedMBTiles, this, &MapView::loadedMBTiles);

    //p->lengthLabel = ui.lengthLabel;
}

bool MapView::canZoomIn()
{
    return (zoom < maxZoomLevel());
}

bool MapView::canZoomOut()
{
    return (zoom > minZoomLevel());
}

void MapView::mouseMovedOverScene(const QPointF& scenePos)
{
    QPointF c = GeoTools::Meters2LatLon(scenePos);

    double a1 = fabs(c.y());
    double d1 = floor(a1);
    double m1 = floor(fmod(a1*60,60));
    double s1 = fmod(a1*3600,60);
    char ns = (c.y() > 0)? 'N' : 'S';

    double a2 = fabs(c.x());
    double d2 = floor(a2);
    double m2 = floor(fmod(a2*60,60));
    double s2 = fmod(a2*3600,60);
    char ew = (c.x() > 0)? 'E' : 'W';

    ui.coordsLabel->setText(coordsTemplate
        .arg(d1,0,'f',0,' ')
        .arg(m1,2,'f',0,'0')
        .arg(s1,5,'f',2,'0').arg(ns)
        .arg(d2,0,'f',0,' ')
        .arg(m2,2,'f',0,'0')
        .arg(s2,5,'f',2,'0').arg(ew));
    emit mouseMoved(GeoTools::Meters2LatLon(scenePos));
}

void MapView::centerScene()
{
    QPointF c = mapToScene(rect().center());
    qreal pw = GeoTools::projectionWidth();
//    if (c.x() > GeoTools::projectionWidth() || c.x() < -GeoTools::projectionWidth())
//        centerOn(fmod(c.x(),GeoTools::projectionWidth()),c.y());

    if (c.x() < -pw/2 || pw/2 < c.x())
    {
        qreal m = fmod(c.x() + pw/2,pw);
        if (m < 0)
            centerOn(pw/2 + m,c.y());
        else
            centerOn(-pw/2 + m,c.y());
    }
}

void MapView::refresh()
{
    tileLayer.clear();
    QPixmapCache::clear();
    updateTiles();
}

void MapView::reload()
{
    tileLayer.reload();
    refresh();
}

void MapView::addPath(Path* p)
{
    p->addToScene(scene);
//    p->centerItem()->setZValue(2);
//    p->centerItem()->setPos(QPointF(0,0));
//    scene->addItem(p->centerItem());
}

bool MapView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        if (showToolTip)
        {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
            QPointF latLon = GeoTools::Meters2LatLon(mapToScene(helpEvent->pos()));
            QToolTip::showText(helpEvent->globalPos(), QString("(%1 ; %2)").arg(latLon.x()).arg(latLon.y()));
        }
        return true;
    }

    if (event->type() == QEvent::Paint)
    {
        if (lastViewportTransform != viewportTransform())
        {
            updateTiles();
            lastViewportTransform = viewportTransform();

            centerScene();
        }
    }
    return QGraphicsView::viewportEvent(event);
}

void MapView::updateTiles()
{
    QRectF drawArea = mapToScene(viewport()->rect().adjusted(-20,-20,20,20)).boundingRect();
    QPoint tl = GeoTools::Meters2GoogleTile(drawArea.topLeft(),zoom);
    QPoint br = GeoTools::Meters2GoogleTile(drawArea.bottomRight(),zoom);
    tileLayer.setRegion(QRect(tl,br),zoom);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    viewAnchor = event->pos();
    sceneAnchor = mapToScene(viewAnchor);
    if (event->modifiers() & Qt::ShiftModifier)
        setDragMode(RubberBandDrag);
    else
        setDragMode(ScrollHandDrag);

    QGraphicsView::mouseMoveEvent(event);

    // Although centerScene is already called in viewportEvent
    // on paint events, and that's enough to keep the scene
    // in place, also doing it here reduces flicker.
    centerScene();
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QMouseEvent e(event->type(),event->pos(),Qt::LeftButton,Qt::MouseButtons(Qt::LeftButton|Qt::RightButton),event->modifiers());
        QGraphicsView::mousePressEvent(&e);
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QMouseEvent e(event->type(),event->pos(),Qt::LeftButton,Qt::MouseButtons(Qt::LeftButton|Qt::RightButton),event->modifiers());
        QGraphicsView::mouseReleaseEvent(&e);
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::leaveEvent(QEvent *event)
{
    viewAnchor = rect().center();
    sceneAnchor = mapToScene(viewAnchor);
    QGraphicsView::leaveEvent(event);
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    viewAnchor = event->pos();
    sceneAnchor = mapToScene(viewAnchor);
    if (event->button() == Qt::LeftButton)
        zoomIn();
    if (event->button() == Qt::RightButton)
        zoomOut();
    //centerOn(sceneAnchor);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    QSettings settings;
    QString defaultWheelOption;
#ifdef Q_OS_MAC
    defaultWheelOption = WheelOptions::ZoomsWithCtrlKeyPressed;
#else
    defaultWheelOption = WheelOptions::ZoomsWithNoKeyPressed;
#endif
    QString wheelOption = settings.value(SettingsKeys::WheelOption, defaultWheelOption).toString();
    bool cond;
    if (wheelOption == WheelOptions::ZoomsWithNoKeyPressed)
    {
        cond = !event->modifiers();
    }
    else //(wheelOption == WheelOptions::ZoomsWithCtrlKeyPressed)
    {
        cond = event->modifiers() == Qt::ControlModifier;
    }
    static int cumulAngleDelta;
    cumulAngleDelta += event->angleDelta().y();
    int zoomAngleStep = 30;
    if (cond)
    {
        if (cumulAngleDelta >= zoomAngleStep)
        {
            cumulAngleDelta = cumulAngleDelta % zoomAngleStep;
            zoomIn();
        }
        else if (cumulAngleDelta <= -zoomAngleStep)
        {
            cumulAngleDelta = -((-cumulAngleDelta) % zoomAngleStep);
            zoomOut();
        }
        event->accept();
        return;
    }

    static qreal cdx = 0;
    static qreal cdy = 0;
    int delta = 0;
    qreal scale = 0.25;

    if (event->orientation() == Qt::Vertical)
    {
        cdy += event->delta() * scale;
        if (qAbs(cdy) > 1)
        {
            delta = (int)cdy;
            cdy = cdy - delta;
        }
    }
    if (event->orientation() == Qt::Horizontal)
    {
        cdx += event->delta() * scale;
        if (qAbs(cdx) > 1)
        {
            delta = (int)cdx;
            cdx = cdx - delta;
        }
    }

    QWheelEvent *e1 = new QWheelEvent(event->pos(), event->globalPos(), delta, event->buttons(), event->modifiers(), event->orientation());
    QGraphicsView::wheelEvent(e1);
    delete e1;

    centerScene();
}

void MapView::contextMenuEvent (QContextMenuEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        QPointF latLon = GeoTools::Meters2LatLon(mapToScene(event->pos()));

        QSignalMapper *signalMapper = new QSignalMapper(this);

        QMenu *menu = new QMenu();

        QMenu *copyMenu = new QMenu("&Copy to clipboard");
        menu->addMenu(copyMenu);

        addMenuAction(signalMapper, copyMenu, QString("(%1;%2)").arg(latLon.y()).arg(latLon.x()));
        addMenuAction(signalMapper, copyMenu, QString("Latitude: %1 ; Longitude: %2").arg(latLon.y()).arg(latLon.x()));
        addMenuAction(signalMapper, copyMenu, QString("Latitude: %1").arg(latLon.y()));
        addMenuAction(signalMapper, copyMenu, QString("Longitude: %1").arg(latLon.x()));
        //QString::fromUtf8("°")

        ////menu->addAction("&Set as center", this, SLOT(setAnchorAsCenter()));
        //QAction *setCenterAction = new QAction("&Set as center", menu);
        //signalMapper->setMapping(setCenterAction, (QObject*)(&latLon));
        //menu->addAction(setCenterAction);

        //connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(setAsCenter(QObject*)));
        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(copyToClipboard(QString)));
        menu->exec(mapToGlobal(event->pos()));

        event->accept();
    }
}

void MapView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift)
    {
        setDragMode(RubberBandDrag);
#ifndef Q_OS_MAC
        setViewportUpdateMode(FullViewportUpdate);
#endif
    }
    else
    {
        setDragMode(ScrollHandDrag);
#ifndef Q_OS_MAC
        setViewportUpdateMode(MinimalViewportUpdate);
#endif
    }
    QGraphicsView::keyPressEvent(event);

    centerScene();
}

void MapView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift)
    {
        setDragMode(ScrollHandDrag);
#ifndef Q_OS_MAC
        setViewportUpdateMode(MinimalViewportUpdate);
#endif
    }
}

void MapView::resizeEvent(QResizeEvent *event)
{
    qDebug() << "MapView::resizeEvent:" << event;
    worldWindow->resize(event->size());
    QGraphicsView::resizeEvent(event);
}

void MapView::setAsCenter(QObject *)
{
    //QPointF viewAnchorScenePos = mapToScene(viewAnchor);
    //centerOn(mapToScene(rect().center()) + sceneAnchor - viewAnchorScenePos);
    //qDebug() << newCenter;
    //centerOn(mapToScene(((QPoint*)newCenter)[0]));
}

void MapView::addMenuAction(QSignalMapper *signalMapper, QMenu *menu, QString text)
{
    QAction *action = new QAction(text, menu);
    signalMapper->setMapping(action, text);
    connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    menu->addAction(action);
}

void MapView::copyToClipboard(QString text)
{
    QApplication::clipboard()->setText(text);
}

void MapView::setZoomLevelCentered(int zoom)
{
    QPoint oldViewAnchor = viewAnchor;
    QPointF oldSceneAnchor = sceneAnchor;
    viewAnchor = rect().center();
    sceneAnchor = mapToScene(viewAnchor);
    setZoomLevel(zoom);
    viewAnchor = oldViewAnchor;
    sceneAnchor = oldSceneAnchor;
}

void MapView::setZoomSliderTooltip(int zoom)
{
    ui.zoomSlider->setToolTip(QString("%1").arg(zoom));
}

void MapView::setCacheStyle(QString cacheStyle)
{
    QSettings settings;
    settings.setValue(SettingsKeys::MapType, cacheStyle);
    refresh();
}

void MapView::setZoomLevel(int zoom)
{
    if (this->zoom == zoom)
        return;
    bool zo = true;
    bool zi = true;
    if (zoom <= 0)
    {
        zoom = 0;
        zo = false;
    }
    if (zoom >= maxZoomLevel())
    {
        zoom = maxZoomLevel();
        zi = false;
    }
    emit canZoomOut(zo);
    emit canZoomIn(zi);
    if (this->zoom == zoom)
        return;
    this->zoom = zoom;
    QSettings settings;
    emit zoomChanged(zoom);
    settings.setValue(SettingsKeys::ZoomLevel, zoom);

    tileLayer.clear();

    double res = GeoTools::resolution(zoom);
    resetTransform();
    scale(1/res,1/res);
    rotate(angle);
    QPointF viewAnchorScenePos = mapToScene(viewAnchor);
    centerOn(mapToScene(rect().center()) + sceneAnchor - viewAnchorScenePos);
    centerScene();
}

void MapView::showLatLonAsToolTip(bool really)
{
    showToolTip = really;
    QSettings settings;
    settings.setValue(SettingsKeys::ShowLatLonAsToolTip, really);
}

void MapView::zoomIn()
{
    setZoomLevel(zoom + 1);
}

void MapView::zoomOut()
{
    setZoomLevel(zoom - 1);
}

int MapView::zoomLevel()
{
    return zoom;
}

int MapView::maxZoomLevel()
{
    return 18;
}

int MapView::minZoomLevel()
{
    return 0;
}

void MapView::rotRight()
{
    rotate(15);
    angle = (angle+15 > 360)? angle - 345 : angle + 15;
    QSettings settings;
    settings.setValue(SettingsKeys::Angle, angle);
}

void MapView::rotLeft()
{
    rotate(-15);
    angle = (angle-15 < 0)? angle + 345 : angle - 15;
    QSettings settings;
    settings.setValue(SettingsKeys::Angle, angle);
}

MapView::~MapView()
{
    QSettings settings;
    QPointF center = GeoTools::Meters2LatLon(mapToScene(rect().center()));
    settings.setValue(SettingsKeys::Latitude, center.y());
    settings.setValue(SettingsKeys::Longitude, center.x());
}
