#include <QtDebug>
#include <QSettings>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QToolTip>
#include <QHBoxLayout>
#include <QSlider>
#include <math.h>

#include "mapview.h"
#include "constants.h"
#include "pathgraphicsitem.h"
#include "geotools.h"
#include "geocircle.h"
#include "worldwindow.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent), scene(new MainScene()), tlayer(scene)
{
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setScene(scene);
    setSceneRect(QRectF(GeoTools::Pixels2Meters(QPointF(INT_MIN,0),0),
                        GeoTools::Pixels2Meters(QPointF(INT_MAX,256),0)));

#ifdef Q_OS_MAC
    setViewportUpdateMode(FullViewportUpdate);
#endif

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

    PathGraphicsItem *p;
    p = new PathGraphicsItem();
    p->setZValue(2);
    //p->setPos(GeoTools::LatLon2Meters(QPointF(-82.38,23.13)));
    scene->addItem(p);

    p = new PathGraphicsItem();
    p->setZValue(2);
    //p->setPos(GeoTools::LatLon2Meters(QPointF(-82.38,23.13)));
    scene->addItem(p);

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
}

bool MapView::canZoomIn()
{
    return (zoom < maxZoomLevel());
}

bool MapView::canZoomOut()
{
    return (zoom > 0);
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
        }
    }
    return QGraphicsView::viewportEvent(event);
}

void MapView::updateTiles()
{
    QRectF drawArea = mapToScene(viewport()->rect().adjusted(-20,-20,20,20)).boundingRect();
    QPoint tl = GeoTools::Meters2GoogleTile(drawArea.topLeft(),zoom);
    QPoint br = GeoTools::Meters2GoogleTile(drawArea.bottomRight(),zoom);
    tlayer.setRegion(QRect(tl,br),zoom);
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
    if (cond)
    {
        if (event->delta() > 0)
        {
            zoomIn();
        }
        else
        {
            zoomOut();
        }
        event->accept();
        return;
    }
    QGraphicsView::wheelEvent(event);
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

void MapView::setCacheStyle(QString cacheStyle)
{
    QSettings settings;
    settings.setValue(SettingsKeys::MapType, cacheStyle);
    tlayer.clear();
    updateTiles();
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

    tlayer.clear();

    double res = GeoTools::resolution(zoom);
    resetTransform();
    scale(1/res,1/res);
    rotate(angle);
    QPointF viewAnchorScenePos = mapToScene(viewAnchor);
    centerOn(mapToScene(rect().center()) + sceneAnchor - viewAnchorScenePos);
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
    return 24;
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
