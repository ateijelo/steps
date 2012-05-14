#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QSettings>
#include <QPixmapCache>

#include "constants.h"
#include "tilelayer.h"

TileLayer::TileLayer()
{
    region.setCoords(0,0,-1,-1);

    fetcherThread = new QThread(this);
    fetcher.moveToThread(fetcherThread);
    connect(&fetcher,SIGNAL(tileData(QString,int,int,int,QByteArray)),
            this,SLOT(tileData(QString,int,int,int,QByteArray)));
    fetcherThread->start();

    tileKeyTemplate = "%1:%2:%3:%4";
    QPixmapCache::setCacheLimit(50*1024);
}

void TileLayer::tileData(const QString &type, int x, int y, int z,
                              const QByteArray &bytes)
{
    qDebug() << "tileData: type =" << type << " x =" << x
            << " y =" << y << " z =" << z << " zoom =" << zoom;
    QPixmap p;
    p.loadFromData(bytes);
    QPixmapCache::insert(Tile::tileKey(type,x,y,z),p);
    if (this->type != type)
        return;
    if (z > zoom)
        return;
    // The tile we've received, which range of tiles does it represent
    // in our current level?
    //QRectF c()
    if (z < zoom)
    {
        foreach (Tile *t, tiles)
        {
            if (t->isWithin(x,y,z))
            {
                qDebug() << "    Using it.";
                t->loadPixmap(p,z);
            }
        }
        return;
    }
    if (tiles.contains(TileCoords(x,y)))
    {
        Tile *t = tiles.value(TileCoords(x,y));
        t->loadPixmap(p,z);
    }
}

Tile* TileLayer::newTile(int x, int y)
{
    Tile *t = new Tile(type,x,y,zoom);
    tiles.insert(t->coords(),t);
    emit tileCreated(t,x,y,zoom);

    QPixmap p;
    for (int z = zoom; z>=0; z--)
    {
        int qx = x >> (zoom - z); // x/(2^(zoom-z))
        int qy = y >> (zoom - z); // y/(2^(zoom-z))
        if (!QPixmapCache::find(Tile::tileKey(type,qx,qy,z),&p))
        {
            qDebug() << "newTile: pixmap cache miss:"
                    << " type =" << type
                    << " qx =" << qx
                    << " qy =" << qy
                    << " z =" << z
                    << " zoom =" << zoom;
            fetcher.fetchTile(type,qx,qy,z);
        }
        else
        {
            qDebug() << "newTile: pixmap cache hit:"
                    << " type =" << type
                    << " qx =" << qx
                    << " qy =" << qy
                    << " z =" << z
                    << " zoom =" << zoom;
            t->loadPixmap(p,z);
            break;
        }
    }

    return t;
}

void TileLayer::deleteTile(Tile *t)
{
    tiles.remove(t->coords());
    fetcher.forgetRequest(t->tileType(),t->tileX(),t->tileY(),t->zoom());
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

void TileLayer::setRegion(const QRect& m, int zoom)
{
    this->zoom = zoom;
    QSettings settings;
    type = settings.value(SettingsKeys::MapType, "").toString();

    //qDebug() << "setRegion: " << n;
    QRect n = m.intersected(QRect(0,0,(1 << zoom),(1 << zoom)));
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
