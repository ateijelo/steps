//#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QSettings>
#include <QPixmapCache>

#include "geotools.h"
#include "constants.h"
#include "tilelayer.h"

TileLayer::TileLayer(QGraphicsScene *scene)
    : scene(scene)
{
    region.setCoords(0,0,-1,-1);

    fetcherThread = new QThread(this);
    fetcher.moveToThread(fetcherThread);
    connect(&fetcher,SIGNAL(tileData(QString,int,int,int,QByteArray)),
            this,SLOT(tileData(QString,int,int,int,QByteArray)));
    connect(this,SIGNAL(fetchTile(QString,int,int,int)),
            &fetcher,SLOT(fetchTile(QString,int,int,int)));
    connect(this,SIGNAL(forgetTile(QString,int,int,int)),
            &fetcher,SLOT(forgetRequest(QString,int,int,int)));
    fetcherThread->start();

    tileKeyTemplate = "%1:%2:%3:%4";
    QPixmapCache::setCacheLimit(50*1024);
}

void TileLayer::tileData(const QString &type, int x, int y, int z,
                              const QByteArray &bytes)
{
    fDebug(DEBUG_TILELAYER) << "tileData: type =" << type << " x =" << x
            << " y =" << y << " z =" << z << " zoom =" << zoom;
    TileId tile(type,x,y,z);
    fetchRequests.remove(tile);
    QPixmap p;
    p.loadFromData(bytes);
    QPixmapCache::insert(Tile::tileKey(type,x,y,z),p);
    if (this->type != type)
        return;
    if (z > zoom)
        return;
    // The tile we've received, which range of tiles does it represent
    // in our current level?
    int dz = zoom - z;
    int ww = (1 << zoom);
    QRect c(x << dz,y << dz,1 << dz,1 << dz);
    QRect r1(modx(region.left()),region.top(),region.width(),region.height());
    QRect r2(modx(region.left())-ww,region.top(),region.width(),region.height());
    QRect i1 = r1.intersected(c);
    QRect i2 = r2.intersected(c);
    setTilesInRegion(i1,p,z);
    setTilesInRegion(i2,p,z);
}

void TileLayer::setTilesInRegion(const QRect &i, const QPixmap p, int z)
{
    int ww = (1 << zoom);
    for (int rx = i.left(); rx <= i.right(); rx++)
    {
        for (int ry = i.top(); ry <= i.bottom(); ry++)
        {
            int leftx = rx - ww * ((rx - region.left() + 1) / ww);
            int rightx = rx + ww * ((region.right() - rx + 1) / ww);
            for (int sx = leftx; sx <= rightx; sx += ww)
            {
                if (tiles.contains(TileCoords(sx,ry)))
                {
                    Tile *t = tiles.value(TileCoords(sx,ry));
                    t->loadPixmap(p,z);
                }
            }
        }
    }
}

Tile* TileLayer::newTile(int x, int y)
{
    int mx = modx(x);
    int my = y;

    Tile *t = new Tile(type,mx,my,zoom);
    tiles.insert(TileCoords(x,y),t);
    double res = GeoTools::resolution(zoom);
    t->setScale(res);
    t->setPos(GeoTools::GoogleTile2Meters(x,y,zoom));
    scene->addItem(t);

    QPixmap p;
    for (int z = zoom; z>=0; z--)
    {
        int qx = mx >> (zoom - z); // x/(2^(zoom-z))
        int qy = my >> (zoom - z); // y/(2^(zoom-z))
        if (!QPixmapCache::find(Tile::tileKey(type,qx,qy,z),&p))
        {
            fDebug(DEBUG_PIXMAPCACHE) << "newTile: pixmap cache miss:"
                    << " type =" << type
                    << " qx =" << qx
                    << " qy =" << qy
                    << " z =" << z
                    << " zoom =" << zoom;
            TileId tile(type,qx,qy,z);
            if (!fetchRequests.contains(tile))
            {
                fetchRequests.insert(tile);
                fDebug(DEBUG_TILELAYER) << "TileLayer.fetchRequests" << fetchRequests;
                emit fetchTile(type,qx,qy,z);
            }
        }
        else
        {
            fDebug(DEBUG_PIXMAPCACHE) << "newTile: pixmap cache hit:"
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

int TileLayer::modx(int x)
{
    int ww = (1 << zoom); // world width in tiles
    // The Fiji feature
    int mx = x % ww; if (mx < 0) mx += ww;
    return mx;
}

void TileLayer::deleteTile(Tile *t)
{
    int ww = (1 << zoom);
    int rx = t->tileX(); int ry = t->tileY();
    int leftx = rx - ww * ((rx - region.left() + 1) / ww);
    int rightx = rx + ww * ((region.right() - rx + 1) / ww);
    for (int sx = leftx; sx <= rightx; sx += ww)
        tiles.remove(TileCoords(sx,ry));

    TileId tile(t->tileType(),t->tileX(),t->tileY(),t->zoom());
    QSet<TileId>::iterator i = fetchRequests.find(tile);
    if (i != fetchRequests.end())
    {
        fetchRequests.erase(i);
        emit forgetTile(tile.type,tile.x,tile.y,tile.zoom);
    }

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

    //fDebug(DEBUG_TILELAYER) << "TileLayer::setRegion: " << m;    
    QRect n = m.intersected(QRect(-(1 << zoom),0,3*(1 << zoom),(1 << zoom)));
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
    while (!fetchRequests.isEmpty())
    {
        QSet<TileId>::iterator i = fetchRequests.begin();
        TileId tile = *i;
        fetchRequests.erase(i);
        emit forgetTile(tile.type,tile.x,tile.y,tile.zoom);
    }
}
