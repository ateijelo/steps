#ifndef PATHEDGE_H
#define PATHEDGE_H

#include <QGraphicsItem>
#include <QLinkedList>

class PathGraphicsItem;
class PathEdgeSegment;
class PathNode;

class PathEdge : public QGraphicsItem
{
    public:
        PathEdge(const QPointF& p1, const QPointF& p2, QGraphicsItem *parent = 0);
        void setParentPath(PathGraphicsItem *path);
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
        double length() const;
        void setP1(const QPointF& p, bool fast=false);
        void setP2(const QPointF& p, bool fast=false);
        void updateSegments(bool fast=false);
        qreal angle1() const;
        qreal angle2() const;

        PathNode *inNode;
        PathNode *outNode;

    private:
        void subdivide(QLinkedList<QPointF>& points, QLinkedList<QPointF>::iterator i,
                       double lat1, double lon1, double azi1, double s1, double s2, int depth);

        bool fastUpdate;
        PathGraphicsItem *parentPath;
        QPointF p1;
        QPointF p2;
        QLinkedList<PathEdgeSegment*> segments;
};

#endif // PATHEDGE_H
