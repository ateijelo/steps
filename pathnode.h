#ifndef PATHNODE_H
#define PATHNODE_H

#include <QGraphicsEllipseItem>

class PathGraphicsItem;
class PathEdge;

class PathNode : public QGraphicsEllipseItem
{
    public:
        PathNode(QGraphicsItem *parent);
        void setParentPath(PathGraphicsItem *path);
        void setExtender(bool b);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
        void hoverEnterEvent(QGraphicsSceneHoverEvent *);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
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

#endif // PATHNODE_H
