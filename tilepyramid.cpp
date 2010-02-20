#include "tilepyramid.h"

TilePyramid::TilePyramid(QGraphicsScene *scene)
        : scene(scene)
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
    int s = layers.size();
    while (s < (z+1))  // If the user requests level n, layers must have n+1 elements
    {
        TileLayer *t = new TileLayer(s,scene);
        t->setRegion(sceneRegion);
        layers.append(t);
        s++;
    }
    while (s > (z+1))  // If the user requests level n, layers must have n+1 elements
    {
        TileLayer *t = layers.takeLast();
        delete t;
        s--;
    }
}
