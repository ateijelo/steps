#ifndef PATHGRAPHICSITEM_H
#define PATHGRAPHICSITEM_H

#include <QGraphicsItem>

class Path;
class PathNode;
class PathEdge;

class PathGraphicsItem : public QGraphicsItem
{
    public:
        explicit PathGraphicsItem(Path *path, QGraphicsItem *parent = 0);
        ~PathGraphicsItem();
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
        void keyPressEvent(QKeyEvent *event);
//        void addEdge(PathNode *from, PathNode *to);
//        void nodeMoved(PathNode *node);
//        void nodeSelectedChanged(PathNode *node, bool selected);
//        void removeNode(PathNode *node);
//        void setPos(const QPointF &pos);
//        void extenderClicked(PathNode *node);
//        void nodeReleased(PathNode *);
//        void edgeHovered(PathEdge *e, const QPointF& pos);
//        void addSibling(PathGraphicsItem *p);
//        double length();
        void updateTail(const QPointF& pos, qreal angle);
        void updateHead(const QPointF& pos, qreal angle);

    signals:

    public slots:

    private:
//        PathNode *newExtenderNode(QGraphicsItem *parent);
//        PathNode *newInnerNode();
//        void updateExtenders();
//        void addNode(const QPointF& pos);

        Path *path;
        QGraphicsLineItem tailExtenderLine;
        QGraphicsLineItem headExtenderLine;
};

#endif // PATHGRAPHICSITEM_H
