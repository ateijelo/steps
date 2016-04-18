#include "pathnode.h"
#include "path.h"
#include "pathnode.h"
#include "pathedge.h"
#include "pathgraphicsitem.h"

//#include "geotools.h"

PathNode::PathNode(Path *path)
    : path(path)
{
    setExtender(false);
    items.reserve(3);
    for (auto& p: path->graphicItems()) {
        items.append(new PathNodeGraphicsItem(this, p));
    }
}

//void PathNode::setParentPath(Path *path)
//{
//    parentPath = path;
//    //setFocusProxy(path);
//}

PathNode::~PathNode()
{
//    for (auto& i: items) {
//        delete i;
//    }
}

QPointF PathNode::pos() const
{
    return items.at(0)->pos();
}

void PathNode::setPos(const QPointF &p)
{
    setPos(p.x(), p.y());
}

void PathNode::setPos(qreal x, qreal y)
{
    for (auto& i: items) {
        i->setPos(x,y);
    }
}

void PathNode::show()
{
    for (auto& i: items) { i->show(); }
}

void PathNode::hide()
{
    for (auto& i: items) { i->hide(); }
}

bool PathNode::hovered() const
{
    return _hovered;
}

bool PathNode::setHovered(bool h)
{
    _hovered = h;
    for (auto& i: items) { i->update(); }
}

bool PathNode::isExtender() const
{
    return _isExtender;
}

void PathNode::setExtender(bool b)
{
    _isExtender = b;
    for (auto& item: items) {
        item->setFlag(item->ItemIsSelectable, !_isExtender);
    }
}

void PathNode::setAcceptHoverEvents(bool b)
{
    for (auto& item: items) {
        item->setAcceptHoverEvents(b);
    }
}

void PathNode::moved(PathNodeGraphicsItem *item)
{
    if (!updates_enabled) return;
    updates_enabled = false;
    for (auto& i: items) {
        if (i != item) {
            i->setPos(item->pos());
        }
    }
    path->nodeMoved(this);
    updates_enabled = true;
}

void PathNode::selectedChanged(PathNodeGraphicsItem *item, bool selected)
{
    if (!updates_enabled) return;
    updates_enabled = false;
    for (auto& i: items) {
        if (i != item) {
            i->setSelected(selected);
        }
    }
    updates_enabled = true;
}

bool PathNode::isSelected() const
{
    return items.at(0)->isSelected();
}

