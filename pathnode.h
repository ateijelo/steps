#ifndef PATHNODE_H
#define PATHNODE_H

#include <QGraphicsEllipseItem>

#include "pathnodegraphicsitem.h"

class PathGraphicsItem;
class PathEdge;
class Path;

class PathNode
{
    public:
        PathNode(Path *path);
        ~PathNode();

        QPointF pos() const;
        void setPos(const QPointF& p);
        void setPos(qreal x, qreal y);
        void show();
        void hide();

        bool hovered() const;
        bool setHovered(bool h);
        bool isExtender() const;
        void setExtender(bool b);
        void setAcceptHoverEvents(bool b);

        void moved(PathNodeGraphicsItem* item);
        void selectedChanged(PathNodeGraphicsItem* item, bool selected);
        bool isSelected() const;

        // parent or sibling objects, not my responsibility to delete
        Path *path;
        PathEdge *inEdge = nullptr;
        PathEdge *outEdge = nullptr;
        PathNode *inNode = nullptr;
        PathNode *outNode = nullptr;

    private:
        bool _isExtender;
        bool _hovered = false;
        bool updates_enabled = true;
        // child objects, it's my responsibility to delete them
        QList<PathNodeGraphicsItem*> items;

};

#endif // PATHNODE_H
