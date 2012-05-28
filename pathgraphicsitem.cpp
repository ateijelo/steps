#include <QtDebug>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <GeographicLib/GeodesicLine.hpp>

#include "pathgraphicsitem.h"
#include "geotools.h"

PathGraphicsItem::PathGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
//    this->setFlag(QGraphicsItem::ItemIsMovable);
    addNode(QPointF(0,0));
    addNode(QPointF(1000,0));
//    addNode(QPointF(2000,0));
//    addNode(QPointF(3000,0));
//    addNode(QPointF(4000,0));
}

void PathGraphicsItem::addNode(const QPointF &pos)
{
    PathNode *n = new PathNode(this);
    n->setPos(pos);
    n->setZValue(2);
    if (!nodes.isEmpty())
    {
        PathNode *m = nodes.last();
        PathEdge *e = new PathEdge(m->pos(),n->pos(),this);
        e->setZValue(1);
        m->outEdge = e;
        n->inEdge = e;
    }
    nodes.append(n);
}

PathGraphicsItem::~PathGraphicsItem()
{
    foreach (PathNode *n, nodes)
    {
        delete n;
    }
}

QRectF PathGraphicsItem::boundingRect() const
{
    return QRectF();
}

void PathGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    return;
}

PathNode::PathNode(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), inEdge(0), outEdge(0)
{
    setBrush(QBrush(Qt::yellow));
    setFlags(QGraphicsItem::ItemIsMovable);
    setCursor(Qt::ArrowCursor);
}

void PathNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 5;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setRect(-width/lod,-width/lod,2*width/lod,2*width/lod);
    setPen(QPen(QBrush(Qt::red),3/lod));

    QGraphicsEllipseItem::paint(painter,option,widget);
}

void PathNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);

    PathEdge *e = 0;

    if (inEdge)
    {
        e = inEdge;
        inEdge->setLine(QLineF(inEdge->line().p1(),pos()));
    }
    if (outEdge)
    {
        e = outEdge;
        outEdge->setLine(QLineF(pos(),outEdge->line().p2()));
    }
    if (e)
    {
        QLineF l = e->line();
        QPointF p1 = e->mapToScene(l.p1());
        QPointF p2 = e->mapToScene(l.p2());
        GeoTools gt;
        p1 = gt.Meters2LatLon(p1);
        p2 = gt.Meters2LatLon(p2);
        const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84;
        double azi1, azi2;
        double a12,s12;
        a12 = g.Inverse(p1.y(),p1.x(),p2.y(),p2.x(),s12,azi1,azi2);
        qDebug() << "p1:" << p1;
        qDebug() << "p2:" << p2;
        qDebug() << "azi1:" << azi1;
        qDebug() << "azi2:" << azi2;
        qDebug() << "a12:" << a12;
        qDebug() << "s12:" << s12;
    }
}

PathEdge::PathEdge(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setLine(QLineF(p1,p2));
}

void PathEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 3;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setPen(QPen(QBrush(Qt::red),width/lod,Qt::SolidLine,Qt::RoundCap));

    QGraphicsLineItem::paint(painter,option,widget);
}
