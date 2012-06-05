#ifndef PATHGRAPHICSITEM_H
#define PATHGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QLinkedList>
#include <QSet>

class PathNode;

class PathEdgeSegment : public QGraphicsLineItem
{
    public:
        PathEdgeSegment(QGraphicsItem *parent = 0);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    private:
        QGraphicsEllipseItem *hoverNode;
};

class PathEdge : public QGraphicsItem
{
    public:
        PathEdge(const QPointF& p1, const QPointF& p2, QGraphicsItem *parent = 0);
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
        double length() const;
        void setP1(const QPointF& p, bool fast=false);
        void setP2(const QPointF& p, bool fast=false);
        void updateSegments(bool fast=false);
        qreal angle1() const;
        qreal angle2() const;

    private:
        void subdivide(QLinkedList<QPointF>& points, QLinkedList<QPointF>::iterator i,
                       double lat1, double lon1, double azi1, double s1, double s2, int depth);

        bool fastUpdate;
        QPointF p1;
        QPointF p2;
        QLinkedList<PathEdgeSegment*> segments;
};

class PathGraphicsItem;

class PathNode : public QGraphicsEllipseItem
{
    public:
        PathNode(QGraphicsItem *parent);
        void setParentPath(PathGraphicsItem *path);
        void setExtender(bool b);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QPainterPath shape() const;

        PathEdge *inEdge;
        PathEdge *outEdge;
        PathNode *inNode;
        PathNode *outNode;
        PathGraphicsItem *parentPath;
        bool isExtender;
        bool hovered;

        QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
};

class PathGraphicsItem : public QGraphicsItem
{
    public:
        explicit PathGraphicsItem(QGraphicsItem *parent = 0);
        ~PathGraphicsItem();
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
        void keyPressEvent(QKeyEvent *event);
        void addNode(const QPointF& pos);
        void addEdge(PathNode *from, PathNode *to);
        void nodeMoved(PathNode *node);
        void nodeSelectedChanged(PathNode *node, bool selected);
        void removeNode(PathNode *node);
        void setPos(const QPointF &pos);
        void extenderClicked(PathNode *node);
        void nodeReleased(PathNode *node);

    signals:

    public slots:

    private:
        PathNode *newExtenderNode(QGraphicsItem *parent);
        void updateExtenders();

        PathNode *head;
        PathNode *tail;
        double length;
        PathNode *tailExtenderNode;
        PathNode *headExtenderNode;
        QGraphicsLineItem tailExtenderLine;
        QGraphicsLineItem headExtenderLine;

};

#endif // PATHGRAPHICSITEM_H
