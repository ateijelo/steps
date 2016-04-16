#ifndef PATHNODE_H
#define PATHNODE_H

#include <QGraphicsEllipseItem>

class PathGraphicsItem;
class PathEdge;
class Path;

class PathNode : public QGraphicsEllipseItem
{
    public:
        PathNode(Path *path, QGraphicsItem *parent);
        void setExtender(bool b);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
        void hoverEnterEvent(QGraphicsSceneHoverEvent *);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QPainterPath shape() const;

        Path *path;
        PathEdge *inEdge;
        PathEdge *outEdge;
        PathNode *inNode;
        PathNode *outNode;
        bool isExtender;
        bool hovered;

        QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
};

#endif // PATHNODE_H
