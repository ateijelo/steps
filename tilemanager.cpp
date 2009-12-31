#include "tilemanager.h"

TileManager::TileManager()
{
    // This class considers the boundaries
    // of the rectangle to be inclusive.
    // So, 0,0,0,0 is a region with one tile.
    region.setCoords(0,0,0,0);
    Column *c = new Column();
    c->append(new Tile(0,0,0));
    columns.append(c);
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

    int check1 = 0; // Only one of the following cycles should enter
    int check2 = 0;

    // Columns to be deleted
    for (int i=o.left(); i<=qMin(o.right(),n.left()-1); i++)
    {
        check1 = 1;
        Column *c = columns.takeFirst();
        deleteColumn(c);
    }

    // Columns to be prepended
    ColumnPointer p = columns.begin();
    for (int i=n.left(); i<=qMin(n.right(),o.left()-1); i++)
    {
        check2 = 1;
        Column *c = new Column();
        // whole columns to prepend
        for (int j=n.top(); j<=n.bottom(); j++)
        {
            c->append(new Tile(i,j,zoom));
        }
        columns.insert(p,c);
    }

    Q_ASSERT_X(check1+check2<2,"beforeInsert","there were inserted and deleted columns");

    return p;
}

void TileManager::adjustColumn(Column* col, const QRect& n, int zoom)
{

}

void TileManager::setRegion(const QRect& n, int zoom)
{
    QRect& o = region; // n => new, o => old

    ColumnPointer p = adjustBeforeIntersection(n,zoom);

    // Only if the following condition holds
    // there's something to do in the intersection
    if (n.top() != o.top() || n.bottom() != o.bottom())
    {
        for (int i=qMax(n.left(),o.left()); i<=qMin(n.right(),o.right()); i++)
        {
        }
    }
    // We're now in the intersection
    // The analysis is similar, but instead of
    // adding or removing whole columns, it'll
    // be individual tiles within one column.
    for (int i=n.top(); i<=qMin(n.bottom(),o.top()-1); i++)
    {
        // tiles to prepend
    }
    for (int i=o.top(); i<=qMin(o.bottom(),n.top()-1); i++)
    {
        // tiles to delete
    }
    // Here the iterator must be moved to the end of the column
    // And a similar process happens
    for (int i=qMax(n.left(),o.right()+1); i<=n.right(); i++)
    {
        // columns to append
    }
}
