#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>

#include "debug.h"

#include "pathedgesegmentgraphicsitem.h"

PathEdgeSegmentGraphicsItem::PathEdgeSegmentGraphicsItem(PathEdgeSegment *segment, QGraphicsItem *parent)
    : segment(segment), QGraphicsLineItem(parent)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    setFlag(ItemIsFocusable);
    setZValue(1);
}

void PathEdgeSegmentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 20;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setPen(QPen(QBrush(QColor(0,0,0,0)),width/lod,Qt::SolidLine,Qt::RoundCap));

    QGraphicsLineItem::paint(painter,option,widget);
    painter->setPen(QPen(QBrush(fEnabled(DEBUG_PATHS) ? segment->randomColor: segment->color),
                         3/lod,Qt::SolidLine,Qt::RoundCap));
    painter->drawLine(line());
//    painter->setPen(QPen(QBrush(Qt::yellow),10/lod,Qt::SolidLine,Qt::RoundCap));
//    painter->drawLine(line());
}

void PathEdgeSegmentGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    if (event->modifiers() & Qt::ControlModifier)
//    {
        QLineF l = line();
        QLineF m = l.normalVector();
        m.translate(event->pos()-m.p1());
        QPointF p;
        l.intersect(m,&p);
        //path->edgeHovered(parentEdge,p);
        segment->hoverMove(p);
        setFocus();
//    }
//    else
//    {
//        hoverNode->hide();
//    }
}

void PathEdgeSegmentGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    clearFocus();
    //hoverNode->hide();
    //path->edgeHovered(0,QPointF());
    segment->hoverLeave();
}
