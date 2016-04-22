#include "pathgraphicsitem.h"
#include "pathedgesegment.h"
#include "pathedgesegmentgraphicsitem.h"

PathEdgeSegment::PathEdgeSegment(PathEdge *edge)
    : edge(edge)
{
    color = Qt::red;
    randomColor.setHsv(qrand() % 256,255,255);

    for (auto& p: edge->pathGraphicsItems()) {
        items.append(new PathEdgeSegmentGraphicsItem(this, p));
    }
    //hoverNode = new QGraphicsEllipseItem(this);
    //hoverNode->setRect(-5,-5,10,10);
    //hoverNode->setFlag(ItemIgnoresTransformations);
    //hoverNode->setFlag(ItemStacksBehindParent);
    //hoverNode->setPen(QPen(QBrush(Qt::yellow),1.5));
    //hoverNode->hide();
}

PathEdgeSegment::~PathEdgeSegment()
{
    foreach (PathEdgeSegmentGraphicsItem *i, items) {
        delete i;
    }
}

qreal PathEdgeSegment::angle()
{
    return items.at(0)->line().angle();
}

void PathEdgeSegment::hoverMove(const QPointF &p) const
{
    edge->hoverMove(p);
}

void PathEdgeSegment::hoverLeave() const
{
    edge->hoverLeave();
}

void PathEdgeSegment::setLine(const QLineF &l)
{
    for (auto& i: items) {
        i->setLine(l);
    }
}

//void PathEdgeSegment::setParentEdge(PathEdge *edge)
//{
//    parentEdge = edge;
//}


