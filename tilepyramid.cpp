#include "tilepyramid.h"

TilePyramid::TilePyramid(QGraphicsItem *parent)
        : QGraphicsItem(parent)
{
}

void TilePyramid::setRegion(const QRectF& sceneRegion)
{
    this->sceneRegion = sceneRegion;
    for (int i=layers.size() - 1; i>=0; i--)
    {
        if (layers.at(i)->wouldChangeWithRegion(sceneRegion))
            layers.at(i)->setRegion(sceneRegion);
        else
            break;
    }
}

void TilePyramid::displayUpToLevel(int z)
{
    int s = layers.size();
    while (s < (z+1))  // If the user requests level n, layers must have n+1 elements (0, 1, 2, ..., n)
    {
        TileLayer *t = new TileLayer(s,this);
        t->setZValue(s);
        layers.append(t);
        s++;
    }
    while (s > (z+1))  // If the user requests level n, layers must have n+1 elements
    {
        TileLayer *t = layers.takeLast();
        delete t;
        if (!layers.isEmpty())
            layers.last()->show();
        s--;
    }
    for (int i=0; i<layers.size() - 1; i++)
        layers.at(i)->hide();
    for (int i=qMax(0,layers.size() - 1); i<layers.size(); i++)
          layers.at(i)->show();
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
