#ifndef PATHEDGESEGMENTGRAPHICSITEM_H
#define PATHEDGESEGMENTGRAPHICSITEM_H

#include <QGraphicsItem>

#include "pathedgesegment.h"

class PathEdgeSegmentGraphicsItem : public QGraphicsLineItem
{
    public:
        PathEdgeSegmentGraphicsItem(PathEdgeSegment *segment, QGraphicsItem *parent = nullptr);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    private:
        QGraphicsEllipseItem *hoverNode;
        PathEdgeSegment *segment;

};

#endif // PATHEDGESEGMENTGRAPHICSITEM_H
