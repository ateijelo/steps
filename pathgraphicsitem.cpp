#include <QPen>
#include <QBrush>
#include <QKeyEvent>
#include <QGraphicsView>

#include "pathgraphicsitem.h"
#include "pathnode.h"
#include "pathedge.h"
#include "debug.h"
#include "path.h"

PathGraphicsItem::PathGraphicsItem(Path *path, QGraphicsItem *parent)
    : QGraphicsItem(parent), path(path)
{
//    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(ItemHasNoContents);
    this->setFlag(ItemIsFocusable);

//    tailExtenderLine.setLine(0,0,35,0);
//    tailExtenderLine.setFlag(ItemIgnoresTransformations);
//    tailExtenderLine.setPen(QPen(QBrush(QColor(255,0,0)),1.5,Qt::SolidLine,Qt::RoundCap));
//    tailExtenderLine.setPos(path->tailPos());
//    tailExtenderLine.setRotation(path->tailAngle());

//    headExtenderLine.setLine(0,0,35,0);
//    headExtenderLine.setFlag(ItemIgnoresTransformations);
//    headExtenderLine.setPen(QPen(QBrush(QColor(255,0,0)),1.5,Qt::SolidLine,Qt::RoundCap));
//    headExtenderLine.setPos(path->headPos());
//    headExtenderLine.setRotation(path->headAngle());

//    tailExtenderNode = newExtenderNode(&tailExtenderLine);
//    headExtenderNode = newExtenderNode(&headExtenderLine);

//    tailExtenderLine.hide();
//    headExtenderLine.hide();

//    innerNode = newInnerNode();
//    innerNodeEdge = nullptr;
}

//PathNode *PathGraphicsItem::newExtenderNode(QGraphicsItem *parent)
//{
//    PathNode *n;
//    n = new PathNode(path, parent);
//    n->setExtender(true);
//    n->setPos(40,0);
//    return n;
//}

//PathNode *PathGraphicsItem::newInnerNode()
//{
//    PathNode *n;
//    n = new PathNode(path, this);
//    n->setExtender(true);
//    n->setAcceptHoverEvents(false);
//    n->hovered = true;
//    n->hide();
//    return n;
//}

//void PathGraphicsItem::updateExtenders()
//{
//    if (head->isSelected())
//        headExtenderLine.show();
//    else
//        headExtenderLine.hide();
//    if (tail->isSelected())
//        tailExtenderLine.show();
//    else
//        tailExtenderLine.hide();
//}

//void PathGraphicsItem::addNode(const QPointF &pos)
//{
//    PathNode *n = new PathNode(path, this);

//    n->setPos(pos);
//    n->setZValue(2);
//    if (head != 0)
//    {
//        PathNode *m = tail;
//        addEdge(m,n);
//    }
//    else
//    {
//        head = n;
//    }
//    tail = n;
//}

PathGraphicsItem::~PathGraphicsItem()
{
//    PathNode *p = head;
//    while (p != 0)
//    {
//        PathNode *q = p;
//        p = p->outNode;
//        delete q;
//    }
}

QRectF PathGraphicsItem::boundingRect() const
{
    return QRectF();
}

void PathGraphicsItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

//void PathGraphicsItem::nodeMoved(PathNode *node)
//{

//}

//void PathGraphicsItem::nodeSelectedChanged(PathNode *node, bool selected)
//{

//}

//void PathGraphicsItem::removeNode(PathNode *node)
//{
//    if (head == tail)
//        return;
//    if (head->outNode == tail)
//        return;
//    if (tail->inNode == head)
//        return;
//    if (node == headExtenderNode)
//        return;
//    if (node == tailExtenderNode)
//        return;
//    if (node == innerNode)
//        return;
//    if (node == head)
//    {
//        head = node->outNode;
//        head->inNode = 0;
//        head->inEdge = 0;
//        _length -= node->outEdge->length();
//        headExtenderLine.setPos(head->pos());
//        headExtenderLine.setRotation(180-head->outEdge->angle1());
//        delete node->outEdge;
//        delete node;
//        return;
//    }
//    if (node == tail)
//    {
//        tail = node->inNode;
//        tail->outNode = 0;
//        tail->outEdge = 0;
//        _length -= node->inEdge->length();
//        tailExtenderLine.setPos(tail->pos());
//        tailExtenderLine.setRotation(-tail->inEdge->angle2());
//        delete node->inEdge;
//        delete node;
//        return;
//    }
//    _length -= node->inEdge->length();
//    _length -= node->outEdge->length();
//    PathNode *n = node->inNode;
//    PathNode *m = node->outNode;
//    delete node->inEdge;
//    delete node;
//    n->outEdge = m->inEdge;
//    n->outEdge->setP1(n->pos());
//    _length += n->outEdge->length();
//    n->outNode = m;
//    m->inNode = n;
//    n->outEdge->inNode = n;
//    headExtenderLine.setRotation(180-head->outEdge->angle1());
//    tailExtenderLine.setRotation(-tail->inEdge->angle2());
//}

//void PathGraphicsItem::setPos(const QPointF &pos)
//{
//    QGraphicsItem::setPos(pos);
//    double d = 0;
//    PathNode *p = head;
//    while (p != 0)
//    {
//        if (p->outEdge != 0)
//        {
//            d += p->outEdge->length();
//        }
//        p = p->outNode;
//    }
//    _length = d;
//}

//void PathGraphicsItem::extenderClicked(PathNode *node)
//{

//}

//void PathGraphicsItem::nodeReleased(PathNode *)
//{
//    PathNode *n = head;
//    while (n)
//    {
//        PathNode *m = n->outNode;
//        if (n->isSelected() && n->outEdge)
//        {
//            n->outEdge->updateSegments(false);
//        }
//        n = m;
//    }
//    updateExtenders();
//}

//void PathGraphicsItem::edgeHovered(PathEdge *e, const QPointF &pos)
//{
//    if (e)
//    {
//        innerNode->setPos(pos);
//        innerNode->show();
//    }
//    else
//    {
//        innerNode->hide();
//    }
//    innerNodeEdge = e;
//}

//double PathGraphicsItem::length()
//{
//    return _length;
//}

void PathGraphicsItem::keyPressEvent(QKeyEvent *event)
{
//    qDebug() << "PathGraphicsItem::keyPressEvent" << event;
//    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
//    {
//        PathNode *n = head;
//        while (n)
//        {
//            PathNode *m = n->outNode;
//            if (n->isSelected())
//                removeNode(n);
//            n = m;
//        }
//        updateExtenders();
//    }
//    else
    //        event->ignore();
}

//void PathGraphicsItem::updateTailExtender(const QPointF &pos, qreal angle)
//{
//    tailExtenderLine.setPos(pos);
//    tailExtenderLine.setRotation(angle);
//}

//void PathGraphicsItem::updateHeadExtender(const QPointF &pos, qreal angle)
//{
//    headExtenderLine.setPos(pos);
//    headExtenderLine.setRotation(angle);
//}

//void PathGraphicsItem::showTailExtender(bool b)
//{
//    if (b) {
//        tailExtenderLine.show();
//    } else {
//        tailExtenderLine.hide();
//    }
//}

//void PathGraphicsItem::showHeadExtender(bool b)
//{
//    if (b) {
//        headExtenderLine.show();
//    } else {
//        headExtenderLine.hide();
//    }
//}
