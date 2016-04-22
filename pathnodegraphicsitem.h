#ifndef PATHNODEGRAPHICSITEM_H
#define PATHNODEGRAPHICSITEM_H

#include <QGraphicsEllipseItem>

class PathNode;

class PathNodeGraphicsItem : public QGraphicsEllipseItem
{
    public:
        PathNodeGraphicsItem(PathNode *node, QGraphicsItem *parent);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
        void hoverEnterEvent(QGraphicsSceneHoverEvent *);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QPainterPath shape() const;
        QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

        void setHovered(bool hovered);

        PathNode *node;
//        PathEdge *inEdge;
//        PathEdge *outEdge;
//        PathNode *inNode;
//        PathNode *outNode;
//        bool isExtender;
};


#endif // PATHNODEGRAPHICSITEM_H
