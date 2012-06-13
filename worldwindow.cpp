#include <QtDebug>
#include <QRegion>
#include <QPainter>
#include <QMouseEvent>

#include "geotools.h"
#include "worldwindow.h"

WorldWindow::WorldWindow(QWidget *parent)
    : QWidget(parent)
{
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    updateMask();
}

void WorldWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    //p.setBrush(QBrush(QColor(255,0,0,128)));
    p.setBrush(Qt::white);
    p.drawRect(rect());
}

void WorldWindow::resize(const QSize &size)
{
    QWidget::resize(size);
    updateMask();
}

//void WorldWindow::resizeEvent(QResizeEvent *event)
//{
//    qDebug() << "WorldWindow::resizeEvent";
//    qDebug() << "    event:" << event;
//    QWidget::resizeEvent(event);
//    updateMask();
//}

void WorldWindow::zoomChanged(int z)
{
    this->z = z;
    updateMask();
}

void WorldWindow::updateMask()
{
    qDebug() << "WorldWindow::updateMask";
    qDebug() << "    width:" << width();
    qDebug() << "    zoom:" << z;

    int worldPixelWidth = 256 * (1 << qBound(0,z,20));
    if (worldPixelWidth > width())
    {
        hide();
        return;
    }
    show();
    int x1 = (width() - worldPixelWidth)/2;
    int x2 = x1 + worldPixelWidth;
    QRegion r(QRect(0,0,x1,height()));
    r = r.united(QRect(x2,0,width(),height()));
    setMask(r);
}
