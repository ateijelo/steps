#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QSettings>
#include <QPixmapCache>

#include "constants.h"
#include "tilelayer.h"

TileLayer::TileLayer()
{
    // This class considers the boundaries
    // of the rectangle to be inclusive.
    // So, 0,0,0,0 is a region with one tile.
    region.setCoords(0,0,-1,-1);
//    Column *c = new Column();
//    c->append(newTile(0,0,0));
//    columns.append(c);
    connect(&fetcher,SIGNAL(tileData(QString,int,int,int,QByteArray)),
            this,SLOT(tileData(QString,int,int,int,QByteArray)));
    tileKeyTemplate = "%1:%2:%3:%4";
}

void TileLayer::tileData(const QString &type, int x, int y, int z,
                              const QByteArray &bytes)
{
    QPixmap p;
    p.loadFromData(bytes);
    QPixmapCache::insert(tileKey(type,x,y,z),p);
    if (z != zoom)
    {
        // this is the place for some pyramid optimization
        return;
    }
    if (tiles.contains(TileCoords(x,y)))
    {
        Tile *t = tiles.value(TileCoords(x,y));
        t->setPixmap(p);
    }
}

QString TileLayer::tileKey(QString type, int x, int y, int z)
{
    return tileKeyTemplate.arg(type).arg(x).arg(y).arg(z);
}

Tile* TileLayer::newTile(int x, int y)
{
    Tile *t = new Tile(x,y,zoom);
    tiles.insert(t->coords(),t);
    emit tileCreated(t,x,y,zoom);

    QPixmap p;
    for (int z = zoom; z>=0; z--)
    {
        int qx = x >> (zoom - z); // x/(2^(zoom-z))
        int qy = y >> (zoom - z); // y/(2^(zoom-z))
        int rx = x % (1 << (zoom - z));
        int ry = y % (1 << (zoom - z));
        if (!QPixmapCache::find(tileKey(type,qx,qy,z),&p))
        {
            fetcher.fetchTile(type,qx,qy,z);
        }
        else
        {
            qDebug() << "newTile: pixmap found in cache."
                    << " x =" << x
                    << " y =" << y
                    << " z =" << z
                    << " zoom =" << zoom;
            int w = 256 >> (zoom - z);
            if (w == 0) w = 1;
            t->setPixmap(p.copy(rx*w,ry*w,w,w).scaled(256,256));
            break;
        }
    }

    return t;
}

void TileLayer::deleteTile(Tile *t)
{
    tiles.remove(t->coords());
    delete t;
}

void TileLayer::deleteColumn(Column *c)
{
    TilePointer p = c->begin();
    while (p!=c->end())
    {
        deleteTile(*p);
        p = c->erase(p);
    }
    delete c;
}

TileLayer::ColumnPointer TileLayer::adjustBeforeIntersection(const QRect& n)
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
            c->append(newTile(i,j));
        }
        columns.insert(p,c);
    }

    return p;
}

void TileLayer::adjustColumn(Column* col, const QRect& n, int x)
{
    QRect& o = region; // n => new, o => old
    TilePointer p;

    // Tiles to be deleted from the beginning of the column
    for (int i=o.top(); i<=qMin(o.bottom(),n.top()-1); i++)
    {
        Tile *t = col->takeFirst();
        deleteTile(t);
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
        deleteTile(t);
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
            c->append(newTile(i,j));
        }
        columns.append(c);
    }
}

void TileLayer::setRegion(const QRect& n, int zoom)
{
    this->zoom = zoom;
    QSettings settings;
    type = settings.value(SettingsKeys::MapType, "").toString();

    //qDebug() << "setRegion: " << n;
    QRect& o = region; // n => new, o => old

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

    region = n;
}

void TileLayer::clear()
{
    setRegion(QRect(0,0,0,0),0);
}
