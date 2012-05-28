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
};

class PathNode : public QGraphicsEllipseItem
{
    public:
        PathNode(QGraphicsItem *parent);

        PathEdge *inEdge;
        PathEdge *outEdge;

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

    signals:

    public slots:

    private:
        QLinkedList<PathEdge*> edges;
        QSet<PathNode*> nodes;
    
};

#endif // PATHGRAPHICSITEM_H
