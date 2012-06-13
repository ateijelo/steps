#include <QtDebug>
#include <QRegion>
#include <QPainter>
#include <QMouseEvent>

#include "geotools.h"
#include "worldwindow.h"
#include "mapview.h"

WorldWindow::WorldWindow(MapView *parent)
    : QWidget(parent), view(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void WorldWindow::paintEvent(QPaintEvent *)
{
    int worldPixelWidth = 256 * (1 << qBound(0,view->zoomLevel(),20));
    if (worldPixelWidth > view->width())
        return;
    QPainter p(this);
    int x1 = (width() - worldPixelWidth)/2;
    int x2 = x1 + worldPixelWidth;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);
    p.drawRect(0,0,x1,height());
    p.drawRect(x2,0,width(),height());
    view->zoomLevel();
}
