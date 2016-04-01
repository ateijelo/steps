#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>

#include "debug.h"
#include "pathgraphicsitem.h"
#include "pathedgesegment.h"

PathEdgeSegment::PathEdgeSegment(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    setFlag(ItemIsFocusable);
    c = Qt::red;
    randomColor.setHsv(qrand() % 256,255,255);

    //hoverNode = new QGraphicsEllipseItem(this);
    //hoverNode->setRect(-5,-5,10,10);
    //hoverNode->setFlag(ItemIgnoresTransformations);
    //hoverNode->setFlag(ItemStacksBehindParent);
    //hoverNode->setPen(QPen(QBrush(Qt::yellow),1.5));
    //hoverNode->hide();
}

void PathEdgeSegment::setParentEdge(PathEdge *edge)
{
    parentEdge = edge;
}

void PathEdgeSegment::setParentPath(PathGraphicsItem *path)
{
    parentPath = path;
}

void PathEdgeSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 20;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setPen(QPen(QBrush(QColor(0,0,0,0)),width/lod,Qt::SolidLine,Qt::RoundCap));

    QGraphicsLineItem::paint(painter,option,widget);
    painter->setPen(QPen(QBrush(fEnabled(DEBUG_PATHS) ? randomColor: c),3/lod,Qt::SolidLine,Qt::RoundCap));
    painter->drawLine(line());
//    painter->setPen(QPen(QBrush(Qt::yellow),10/lod,Qt::SolidLine,Qt::RoundCap));
//    painter->drawLine(line());
}

void PathEdgeSegment::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    if (event->modifiers() & Qt::ControlModifier)
//    {
        QLineF l = line();
        QLineF m = l.normalVector();
        m.translate(event->pos()-m.p1());
        QPointF p;
        l.intersect(m,&p);
        parentPath->edgeHovered(parentEdge,p);
        setFocus();
//    }
//    else
//    {
//        hoverNode->hide();
//    }
}

void PathEdgeSegment::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    clearFocus();
    //hoverNode->hide();
    parentPath->edgeHovered(0,QPointF());
}
