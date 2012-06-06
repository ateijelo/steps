#ifndef PATHEDGESEGMENT_H
#define PATHEDGESEGMENT_H

#include <QGraphicsItem>

#include "pathedge.h"
#include "pathnode.h"

class PathGraphicsItem;

class PathEdgeSegment : public QGraphicsLineItem
{
    public:
        PathEdgeSegment(QGraphicsItem *parent = 0);
        void setParentPath(PathGraphicsItem *path);
        void setParentEdge(PathEdge *edge);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    private:
        QGraphicsEllipseItem *hoverNode;
        PathGraphicsItem *parentPath;
        PathEdge *parentEdge;
        QColor c;
};


#endif // PATHEDGESEGMENT_H
