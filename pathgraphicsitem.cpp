#include <QtDebug>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "pathgraphicsitem.h"

PathGraphicsItem::PathGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
//    this->setFlag(QGraphicsItem::ItemIsMovable);
    PathNode *n1 = new PathNode(this);
    PathNode *n2 = new PathNode(this);
    n1->setPos(0,0);
    n2->setPos(1000,1000);
    PathEdge *e = new PathEdge(n1->pos(),n2->pos(),this);
    nodes.insert(n1);
    nodes.insert(n2);
    n1->outEdge = e;
    n2->inEdge = e;
    edges.append(e);
    n1->setZValue(2);
    n2->setZValue(2);
    e->setZValue(1);
}

PathGraphicsItem::~PathGraphicsItem()
{
    foreach (QGraphicsLineItem *e, edges)
    {
        delete e;
    }
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
    setBrush(QBrush(Qt::red));
    setFlags(QGraphicsItem::ItemIsMovable);
    setCursor(Qt::ArrowCursor);
}

void PathNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 5;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setRect(-width/lod,-width/lod,2*width/lod,2*width/lod);

    QGraphicsEllipseItem::paint(painter,option,widget);
}

void PathNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);

    if (inEdge)
    {
        inEdge->setLine(QLineF(inEdge->line().p1(),pos()));
    }
    if (outEdge)
    {
        outEdge->setLine(QLineF(pos(),outEdge->line().p2()));
    }
}

PathEdge::PathEdge(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setLine(QLineF(p1,p2));
}

void PathEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 4;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setPen(QPen(QBrush(Qt::red),width/lod,Qt::SolidLine,Qt::RoundCap));

    QGraphicsLineItem::paint(painter,option,widget);
}
