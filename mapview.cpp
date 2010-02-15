#include <QtDebug>
#include <QSettings>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QToolTip>

#include "mapview.h"
#include "constants.h"

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    scene = new MainScene();
    setScene(scene);
    setSceneRect(QRectF(gt.Pixels2Meters(QPointF(0,0),0),
                        gt.Pixels2Meters(QPointF(256,256),0)));

    connect(&tm,SIGNAL(tileCreated(Tile*,int,int,int)),this,SLOT(displayNewTile(Tile*,int,int,int)));
    connect(scene,SIGNAL(mouseMoved(QPointF)),this,SLOT(mouseMovedOverScene(QPointF)));

    QSettings settings;
    GeoTools gt;

    angle = settings.value(SettingsKeys::Angle, 0.0F).toDouble();

    zoom = -1;
    setZoomLevel(settings.value(SettingsKeys::ZoomLevel, 0).toInt());

    qreal lat = settings.value(SettingsKeys::Latitude, 0).toDouble();
    qreal lon = settings.value(SettingsKeys::Longitude, 0).toDouble();
    centerOn(gt.LatLon2Meters(QPointF(lon,lat)));
}

bool MapView::canZoomIn()
{
    return (zoom < 18);
}

bool MapView::canZoomOut()
{
    return (zoom > 0);
}

void MapView::mouseMovedOverScene(const QPointF& scenePos)
{
    emit mouseMoved(gt.Meters2LatLon(scenePos));
}

bool MapView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        updateTiles();
    }
    else if (event->type() == QEvent::ToolTip)
    {
         QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
         GeoTools gt;
         QPointF latLon = gt.Meters2LatLon(mapToScene(helpEvent->pos()));
         QToolTip::showText(helpEvent->globalPos(), QString("(%1 ; %2)").arg(latLon.x()).arg(latLon.y()));
         return true;
    }
    return QGraphicsView::viewportEvent(event);
}

void MapView::updateTiles()
{
    QRectF drawArea = mapToScene(viewport()->rect().adjusted(-20,-20,20,20)).boundingRect();
    QPoint tl = gt.Meters2GoogleTile(drawArea.topLeft(),zoom);
    QPoint br = gt.Meters2GoogleTile(drawArea.bottomRight(),zoom);
    tm.setRegion(QRect(tl,br),zoom);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    viewAnchor = event->pos();
    sceneAnchor = mapToScene(viewAnchor);
    QGraphicsView::mouseMoveEvent(event);
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
        QPointF latLon = gt.Meters2LatLon(mapToScene(event->pos()));

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

void MapView::setAsCenter(QObject *newCenter)
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

void MapView::setMapType2GoogleMap()
{
    QSettings settings;
    settings.setValue(SettingsKeys::MapType, MapTypes::GoogleMap);
    tm.clear();
    updateTiles();
}

void MapView::setMapType2GoogleSat()
{
    QSettings settings;
    settings.setValue(SettingsKeys::MapType, MapTypes::GoogleSat);
    tm.clear();
    updateTiles();
}

void MapView::setMapType2GoogleHyb()
{
    QSettings settings;
    settings.setValue(SettingsKeys::MapType, MapTypes::GoogleHyb);
    tm.clear();
    updateTiles();
}

void MapView::displayNewTile(Tile *t, int x, int y, int zoom)
{
    t->resetTransform();
    double res = gt.resolution(zoom);
#if QT_VERSION >= 0x040600
    t->setScale(res);
#else
    t->scale(res,res);
#endif
    t->setPos(gt.GoogleTile2Meters(x,y,zoom));
    scene->addItem(t);
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
    if (zoom >= 18)
    {
        zoom = 18;
        zi = false;
    }
    emit canZoomOut(zo);
    emit canZoomIn(zi);
    this->zoom = zoom;
    QSettings settings;
    settings.setValue(SettingsKeys::ZoomLevel, zoom);
    emit zoomChanged(zoom);

    tm.clear();

    double res = gt.resolution(zoom);
    resetTransform();
    scale(1/res,1/res);
    rotate(angle);
    QPointF viewAnchorScenePos = mapToScene(viewAnchor);
    centerOn(mapToScene(rect().center()) + sceneAnchor - viewAnchorScenePos);
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
    QPointF center = gt.Meters2LatLon(mapToScene(rect().center()));
    settings.setValue(SettingsKeys::Latitude, center.y());
    settings.setValue(SettingsKeys::Longitude, center.x());
}
