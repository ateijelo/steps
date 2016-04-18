#ifndef PATHEDGESEGMENT_H
#define PATHEDGESEGMENT_H

#include "path.h"
#include "pathedge.h"

class PathEdgeSegmentGraphicsItem;

class PathEdgeSegment
{
    public:
        PathEdgeSegment(PathEdge *edge);
        ~PathEdgeSegment();
        QColor color;
        QColor randomColor;
        qreal angle();
        void hoverMove(const QPointF& p) const;
        void hoverLeave() const;
        void setLine(const QLineF &l);

    private:
        PathEdge *edge;
        QList<PathEdgeSegmentGraphicsItem*> items;
};


#endif // PATHEDGESEGMENT_H
