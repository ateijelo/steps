#include <QtDebug>

#include "tilemanager.h"

TileManager::TileManager()
{
    // This class considers the boundaries
    // of the rectangle to be inclusive.
    // So, 0,0,0,0 is a region with one tile.
    region.setCoords(0,0,-1,-1);
//    Column *c = new Column();
//    c->append(newTile(0,0,0));
//    columns.append(c);
}

Tile* TileManager::newTile(int x, int y, int zoom)
{
    Tile *t = new Tile(x,y,zoom);
    emit tileCreated(t,x,y,zoom);
    return t;
}

void TileManager::deleteColumn(Column *c)
{
    TilePointer p = c->begin();
    while (p!=c->end())
    {
        delete (*p);
        p = c->erase(p);
    }
    delete c;
}

TileManager::ColumnPointer TileManager::adjustBeforeIntersection(const QRect& n, int zoom)
{
    QRect& o = region; // n => new, o => old

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
            c->append(newTile(i,j,zoom));
        }
        columns.insert(p,c);
    }

    return p;
}

void TileManager::adjustColumn(Column* col, const QRect& n, int x, int zoom)
{
    QRect& o = region; // n => new, o => old
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
        col->insert(p,newTile(x,i,zoom));
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
        col->insert(p,newTile(x,i,zoom));
    }
}

void TileManager::adjustAfterIntersection(const QRect& n, int zoom)
{
    QRect& o = region; // n => new, o => old
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
            c->append(newTile(i,j,zoom));
        }
        columns.append(c);
    }
}

void TileManager::setRegion(const QRect& n, int zoom)
{
    qDebug() << "setRegion: " << n;
    QRect& o = region; // n => new, o => old

    ColumnPointer p = adjustBeforeIntersection(n,zoom);

    // Only if the following condition holds
    // there's something to do in the intersection
    if (n.top() != o.top() || n.bottom() != o.bottom())
    {
        for (int i=qMax(n.left(),o.left()); i<=qMin(n.right(),o.right()); i++)
        {
            adjustColumn(*p,n,i,zoom);
            p++;
        }
    }

    adjustAfterIntersection(n,zoom);

    region = n;
}

void TileManager::clear()
{
    setRegion(QRect(0,0,0,0),0);
}
