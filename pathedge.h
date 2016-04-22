#ifndef PATHEDGE_H
#define PATHEDGE_H

#include <QGraphicsItem>
#include <QLinkedList>

class PathGraphicsItem;
class PathEdgeSegment;
class PathNode;
class Path;

class PathEdge
{
    public:
        PathEdge(const QPointF& p1, const QPointF& p2, Path *path);
        ~PathEdge();
//        QRectF boundingRect() const;
//        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
        double length() const;
        void setP1(const QPointF& p, bool fast=false);
        void setP2(const QPointF& p, bool fast=false);
        void updateSegments(bool fast=false);
        void replaceSegments(const QLinkedList<QPointF> &points);
        void addSibling(PathEdge *e);
        const QList<PathGraphicsItem*>& pathGraphicsItems() const;
        qreal angle1() const;
        qreal angle2() const;
        void hoverMove(const QPointF& p);
        void hoverLeave();

        PathNode *inNode;
        PathNode *outNode;


    private:
        void subdivide(QLinkedList<QPointF>& points, QLinkedList<QPointF>::iterator i,
                       double lat1, double lon1, double azi1, double s1, double s2, int depth);

        bool fastUpdate;
        Path *path;
        QPointF p1;
        QPointF p2;
        QLinkedList<PathEdgeSegment*> segments;
};

#endif // PATHEDGE_H
