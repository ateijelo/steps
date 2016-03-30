#ifndef PATHGRAPHICSITEM_H
#define PATHGRAPHICSITEM_H

#include <QGraphicsItem>

class Path;
class PathNode;
class PathEdge;

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
        void nodeReleased(PathNode *);
        void edgeHovered(PathEdge *e, const QPointF& pos);
        void setPath(Path *path);

    signals:

    public slots:

    private:
        PathNode *newExtenderNode(QGraphicsItem *parent);
        PathNode *newInnerNode(QGraphicsItem *parent);
        void updateExtenders();

        PathNode *head;
        PathNode *tail;
        double length;
        PathNode *tailExtenderNode;
        PathNode *headExtenderNode;
        PathNode *innerNode;
        PathEdge *innerNodeEdge;
        QGraphicsLineItem tailExtenderLine;
        QGraphicsLineItem headExtenderLine;
        QList<PathGraphicsItem*> siblings;

        Path *path;

};

#endif // PATHGRAPHICSITEM_H
