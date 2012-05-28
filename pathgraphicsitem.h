#ifndef PATHGRAPHICSITEM_H
#define PATHGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QLinkedList>
#include <QSet>

class PathEdge : public QGraphicsLineItem
{
    public:
        PathEdge(const QPointF& p1, const QPointF& p2, QGraphicsItem *parent = 0);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        double length() const; 
};

class PathGraphicsItem;

class PathNode : public QGraphicsEllipseItem
{
    public:
        PathNode(PathGraphicsItem *parent);

        PathEdge *inEdge;
        PathEdge *outEdge;
        PathNode *inNode;
        PathNode *outNode;
        PathGraphicsItem *parent;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

class PathGraphicsItem : public QGraphicsItem
{
    public:
        explicit PathGraphicsItem(QGraphicsItem *parent = 0);
        ~PathGraphicsItem();
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void addNode(const QPointF& pos);
        void nodeMoved(PathNode *node);

    signals:

    public slots:

    private:
        PathNode *head;
        PathNode *tail;
        double length;
};

#endif // PATHGRAPHICSITEM_H
