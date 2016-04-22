#include <QtDebug>

#include "pathnode.h"
#include "path.h"
#include "pathnode.h"
#include "pathedge.h"
#include "pathgraphicsitem.h"

//#include "geotools.h"

PathNode::PathNode(Path *path, const QList<PathGraphicsItem*>& parents)
    : path(path)
{
    items.reserve(3);
    for (auto& p: parents) {
        items.append(new PathNodeGraphicsItem(this, p));
    }
    setExtender(false);
}

PathNode::PathNode(Path *path, const QList<QGraphicsLineItem *> &parents)
    : path(path)
{
    items.reserve(3);
    for (auto& p: parents) {
        items.append(new PathNodeGraphicsItem(this, p));
    }
    setExtender(false);
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

void PathNode::setParents(const QList<PathGraphicsItem *> &parents)
{
    if (parents.size() != items.size())
        return;
    for (int i=0; i < parents.size(); i++)
        items.at(i)->setParentItem(parents.at(i));
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

void PathNode::setHovered(bool h)
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
    if (!_isExtender)
        path->nodeMoved(this);
    updates_enabled = true;
}

void PathNode::selectedChanged(PathNodeGraphicsItem *item, bool selected)
{
    qDebug() << "PathNode::selectedChanged(" << item << ", " << selected << ")";
    if (!updates_enabled) return;
    updates_enabled = false;
    for (auto& i: items) {
        if (i != item) {
            i->setSelected(selected);
        }
    }
    if (!_isExtender) {
        path->nodeSelectedChanged(this, selected);
    }
    updates_enabled = true;
}

void PathNode::setSelected(bool b)
{
    updates_enabled = false;
    for (auto& i: items) {
        i->setSelected(b);
    }
    updates_enabled = true;
}

bool PathNode::isSelected() const
{
    return items.at(0)->isSelected();
}

void PathNode::clicked(Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
{
    if (button == Qt::LeftButton && _isExtender)
    {
        path->extenderClicked(this);
    }
    if (button == Qt::LeftButton && (modifiers & Qt::ShiftModifier))
    {
        //path->removeNode(this);
    }
    if (button == Qt::MiddleButton)
    {
        //path->removeNode(this);
    }
}

QTransform PathNode::deviceTransform(const QTransform &viewportTransform)
{
    return items.at(0)->deviceTransform(viewportTransform);
}

