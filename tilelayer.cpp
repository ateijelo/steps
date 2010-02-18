#include <QtDebug>

#include "tilelayer.h"

TileLayer::TileLayer(int zoom, QGraphicsItem *parent)
        : QGraphicsItem(parent), zoom(zoom)
{
    // This class considers the boundaries
    // of the tileRegion rectangle to be inclusive
    // So, setCoords(0,0,0,0) sets a tile region of one tile.
    // Note that width and height would be 1 in that
    // case.
    tileRegion.setCoords(0,0,-1,-1);
#if QT_VERSION >= 0x040600
    setFlag(QGraphicsItem::ItemHasNoContents);
#endif
}

Tile *TileLayer::newTile(int x, int y)
{
    Tile *t = new Tile(x,y,zoom);
    t->setParentItem(this);
    double res = gt.resolution(zoom);
#if QT_VERSION >= 0x040600
    t->setScale(res);
#else
    t->scale(res,res);
#endif
    t->setPos(gt.GoogleTile2Meters(x,y,zoom));
    return t;
}

void TileLayer::deleteColumn(Column *c)
{
    TilePointer p = c->begin();
    while (p!=c->end())
    {
        delete (*p);
        p = c->erase(p);
    }
    delete c;
}

TileLayer::ColumnPointer TileLayer::adjustBeforeIntersection(const QRect& n)
{
    QRect& o = tileRegion; // n => new, o => old

    // Columns to be deleted from front
    for (int i=o.left(); i<=qMin(o.right(),n.left()-1); i++)
    {
        Column *c = columns.takeFirst();
        deleteColumn(c);
    }

    // Columns to be prepended
    ColumnPointer p = columns.begin();
    for (int i=n.left(); i<=qMin(n.right(),o.left()-1); i++)
    {
        Column *c = new Column();
        for (int j=n.top(); j<=n.bottom(); j++)
        {
            c->append(newTile(i,j));
        }
        columns.insert(p,c);
    }

    return p;
}

void TileLayer::adjustColumn(Column* col, const QRect& n, int x)
{
    QRect& o = tileRegion; // n => new, o => old
    TilePointer p;

    // Tiles to be deleted from the beginning of the column
    for (int i=o.top(); i<=qMin(o.bottom(),n.top()-1); i++)
    {
        Tile *t = col->takeFirst();
        delete t;
    }
    // Tiles to be prepended in the beginning of the column
    p = col->begin();
    for (int i=n.top(); i<=qMin(n.bottom(),o.top()-1); i++)
    {
        col->insert(p,newTile(x,i));
    }

    // Tiles to be deleted from the end of the column
    for (int i=qMax(n.bottom()+1,o.top()); i<=o.bottom(); i++)
    {
        Tile *t = col->takeLast();
        delete t;
    }
    // Tiles to be appended to the end of the column
    p = col->end();
    for (int i=qMax(o.bottom()+1,n.top()); i<=n.bottom(); i++)
    {
        col->insert(p,newTile(x,i));
    }
}

void TileLayer::adjustAfterIntersection(const QRect& n)
{
    QRect& o = tileRegion; // n => new, o => old
    for (int i=qMax(o.left(),n.right()+1); i<=o.right(); i++)
    {
        Column *c = columns.takeLast();
        deleteColumn(c);
    }
    for (int i=qMax(n.left(),o.right()+1); i<=n.right(); i++)
    {
        Column *c = new Column();
        for (int j=n.top(); j<=n.bottom(); j++)
        {
            c->append(newTile(i,j));
        }
        columns.append(c);
    }
}

void TileLayer::setRegion(const QRectF& sceneRegion)
{
    //qDebug() << "setRegion: " << n;
    QRect& o = tileRegion; // n => new, o => old
    QRect n(gt.Meters2GoogleTile(sceneRegion.topLeft(),zoom),
            gt.Meters2GoogleTile(sceneRegion.bottomRight(),zoom));

    ColumnPointer p = adjustBeforeIntersection(n);

    // Only if the following condition holds
    // there's something to do in the intersection
    if (n.top() != o.top() || n.bottom() != o.bottom())
    {
        for (int i=qMax(n.left(),o.left()); i<=qMin(n.right(),o.right()); i++)
        {
            adjustColumn(*p,n,i);
            p++;
        }
    }

    adjustAfterIntersection(n);

    tileRegion = n;
}

void TileLayer::clear()
{
}

QRectF TileLayer::boundingRect() const
{
    QPointF tl = gt.GoogleTile2Meters(tileRegion.topLeft(),zoom);
    QPointF br = gt.GoogleTile2Meters(tileRegion.bottomRight() + QPoint(1,1),zoom);
    return QRectF(tl,br);
}

void TileLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
