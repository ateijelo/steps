#include <QtDebug>
#include <QSettings>

#include "mapview.h"

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
    angle = 0;
    zoom = -1;
    //setZoomLevel(0);
    //updateTiles();
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
    //qDebug() << "MapView::mouseMoveEvent" << event->pos();
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
    if ((event->modifiers() & Qt::ControlModifier))
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

void MapView::setMapStyle()
{
    QSettings settings;
    settings.setValue("TileStyle", TILE_STYLE_MAP);
    tm.clear();
    updateTiles();
}

void MapView::setSatStyle()
{
    QSettings settings;
    settings.setValue("TileStyle", TILE_STYLE_SAT);
    tm.clear();
    updateTiles();
}

void MapView::setHybStyle()
{
    QSettings settings;
    settings.setValue("TileStyle", TILE_STYLE_HYB);
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

void MapView::rotRight()
{
    rotate(15);
    angle = (angle+15 > 360)? angle - 345 : angle + 15;
}

void MapView::rotLeft()
{
    rotate(-15);
    angle = (angle-15 < 0)? angle + 345 : angle - 15;
}
