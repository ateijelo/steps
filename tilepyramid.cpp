#include "tilepyramid.h"

TilePyramid::TilePyramid(QGraphicsItem *parent)
        : QGraphicsItem(parent)
{
}

void TilePyramid::setRegion(const QRectF& sceneRegion)
{
    this->sceneRegion = sceneRegion;
    foreach (TileLayer *t, layers)
        t->setRegion(sceneRegion);
}

void TilePyramid::displayUpToLevel(int z)
{
    while (layers.size() < z)
    {
        TileLayer *t = new TileLayer(layers.size(),this);
        t->setZValue(layers.size());
        t->setRegion(sceneRegion);
        layers.append(t);
    }
    while (layers.size() > z)
    {
        TileLayer *t = layers.takeLast();
        delete t;
    }
}

QRectF TilePyramid::boundingRect() const
{
    QPointF tl = gt.GoogleTile2Meters(0,0,0);
    QPointF br = gt.GoogleTile2Meters(1,1,0);
    return QRectF(tl,br);
}

void TilePyramid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
