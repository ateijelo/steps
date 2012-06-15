#include <QPen>
#include <QBrush>
#include <QKeyEvent>
#include <QGraphicsView>

#include "pathgraphicsitem.h"
#include "pathnode.h"
#include "pathedge.h"
#include "debug.h"

PathGraphicsItem::PathGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent), head(0), tail(0), length(0.0),
      tailExtenderLine(this), headExtenderLine(this)
{
//    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(ItemHasNoContents);
    this->setFlag(ItemIsFocusable);

    //for (int i=0; i<2; i++)
    addNode(QPointF(-6.46431e+06, 1.00894e+07));
    addNode(QPointF(1.52169e+07, 2.88845e+06));

    tailExtenderLine.setLine(0,0,35,0);
    tailExtenderLine.setFlag(ItemIgnoresTransformations);
    tailExtenderLine.setPen(QPen(QBrush(QColor(255,0,0)),1.5,Qt::SolidLine,Qt::RoundCap));
    tailExtenderLine.setPos(tail->pos());
    tailExtenderLine.setRotation(-tail->inEdge->angle2());

    headExtenderLine.setLine(0,0,35,0);
    headExtenderLine.setFlag(ItemIgnoresTransformations);
    headExtenderLine.setPen(QPen(QBrush(QColor(255,0,0)),1.5,Qt::SolidLine,Qt::RoundCap));
    headExtenderLine.setPos(head->pos());
    headExtenderLine.setRotation(180-head->outEdge->angle1());

    tailExtenderNode = newExtenderNode(&tailExtenderLine);
    headExtenderNode = newExtenderNode(&headExtenderLine);

    tailExtenderLine.hide();
    headExtenderLine.hide();

    innerNode = newInnerNode(this);
    innerNodeEdge = 0;
}

PathNode *PathGraphicsItem::newExtenderNode(QGraphicsItem *parent)
{
    PathNode *n;
    n = new PathNode(parent);
    n->setZValue(2);
    n->setParentPath(this);
    n->setExtender(true);
    n->setPos(40,0);
    return n;
}

PathNode *PathGraphicsItem::newInnerNode(QGraphicsItem *parent)
{
    PathNode *n;
    n = new PathNode(parent);
    n->setZValue(2);
    n->setParentPath(this);
    n->setExtender(true);
    n->setAcceptsHoverEvents(false);
    n->hovered = true;
    n->hide();
    return n;
}

void PathGraphicsItem::updateExtenders()
{
    if (head->isSelected())
        headExtenderLine.show();
    else
        headExtenderLine.hide();
    if (tail->isSelected())
        tailExtenderLine.show();
    else
        tailExtenderLine.hide();
}

void PathGraphicsItem::addNode(const QPointF &pos)
{
    PathNode *n = new PathNode(this);
    n->setParentPath(this);

    n->setPos(pos);
    n->setZValue(2);
    if (head != 0)
    {
        PathNode *m = tail;
        addEdge(m,n);
    }
    else
    {
        head = n;
    }
    tail = n;
}

void PathGraphicsItem::addEdge(PathNode *from, PathNode *to)
{
    PathEdge *e = new PathEdge(from->pos(),to->pos(),this);
    e->setZValue(1);
    e->inNode = from;
    e->outNode = to;
    from->outEdge = e;
    to->inEdge = e;
    from->outNode = to;
    to->inNode = from;
    length += e->length();
}

PathGraphicsItem::~PathGraphicsItem()
{
    PathNode *p = head;
    while (p != 0)
    {
        PathNode *q = p;
        p = p->outNode;
        delete q;
    }
}

QRectF PathGraphicsItem::boundingRect() const
{
    return QRectF();
}

void PathGraphicsItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void PathGraphicsItem::nodeMoved(PathNode *node)
{
    if (node->inEdge)
    {
        length -= node->inEdge->length();
        bool fast = false;
        if (node->inNode->isSelected())
            fast = true;
        node->inEdge->setP2(node->pos(),fast);
        length += node->inEdge->length();
    }
    if (node->outEdge)
    {
        length -= node->outEdge->length();
        bool fast = false;
        if (node->outNode->isSelected())
            fast = true;
        node->outEdge->setP1(node->pos(),fast);
        length += node->outEdge->length();
    }
    fDebug(DEBUG_PATHS) << "    length: " << length;
    if (node == tail)
    {
        tailExtenderLine.setPos(tail->pos());
        tailExtenderLine.setRotation(-tail->inEdge->angle2());
    }
    if (node->outNode == tail)
        tailExtenderLine.setRotation(-tail->inEdge->angle2());
    if (node == head)
    {
        headExtenderLine.setPos(head->pos());
        headExtenderLine.setRotation(180-head->outEdge->angle1());
    }
    if (node->inNode == head)
        headExtenderLine.setRotation(180-head->outEdge->angle1());
}

void PathGraphicsItem::nodeSelectedChanged(PathNode *node, bool selected)
{
    if (node == tail)
        tailExtenderLine.setVisible(selected);
    if (node == head)
        headExtenderLine.setVisible(selected);
    fDebug(DEBUG_PATHS) << "    length:" << length;
}

void PathGraphicsItem::removeNode(PathNode *node)
{
    if (head == tail)
        return;
    if (head->outNode == tail)
        return;
    if (tail->inNode == head)
        return;
    if (node == headExtenderNode)
        return;
    if (node == tailExtenderNode)
        return;
    if (node == innerNode)
        return;
    if (node == head)
    {
        head = node->outNode;
        head->inNode = 0;
        head->inEdge = 0;
        length -= node->outEdge->length();
        headExtenderLine.setPos(head->pos());
        headExtenderLine.setRotation(180-head->outEdge->angle1());
        delete node->outEdge;
        delete node;
        return;
    }
    if (node == tail)
    {
        tail = node->inNode;
        tail->outNode = 0;
        tail->outEdge = 0;
        length -= node->inEdge->length();
        tailExtenderLine.setPos(tail->pos());
        tailExtenderLine.setRotation(-tail->inEdge->angle2());
        delete node->inEdge;
        delete node;
        return;
    }
    length -= node->inEdge->length();
    length -= node->outEdge->length();
    PathNode *n = node->inNode;
    PathNode *m = node->outNode;
    delete node->inEdge;
    delete node;
    n->outEdge = m->inEdge;
    n->outEdge->setP1(n->pos());
    length += n->outEdge->length();
    n->outNode = m;
    m->inNode = n;
    n->outEdge->inNode = n;
    headExtenderLine.setRotation(180-head->outEdge->angle1());
    tailExtenderLine.setRotation(-tail->inEdge->angle2());
}

void PathGraphicsItem::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos);
    double d = 0;
    PathNode *p = head;
    while (p != 0)
    {
        if (p->outEdge != 0)
        {
            d += p->outEdge->length();
        }
        p = p->outNode;
    }
    length = d;
}

void PathGraphicsItem::extenderClicked(PathNode *node)
{
    QPointF p;
    QList<QGraphicsView*> l = scene()->views();
    if (l.size() > 0)
    {
        QGraphicsView *v = l.at(0);
        fDebug(DEBUG_PATHS) << "viewportTransform:" << v->viewportTransform();
        fDebug(DEBUG_PATHS) << "node->deviceTransform:" << node->deviceTransform(v->viewportTransform());
        fDebug(DEBUG_PATHS) << "  .map:" << node->deviceTransform(v->viewportTransform()).map(QPointF(0,0));
        QPointF q = node->deviceTransform(v->viewportTransform()).map(QPointF(0,0));
        fDebug(DEBUG_PATHS) << "  deviceTransform.map:" << deviceTransform(v->viewportTransform().inverted()).map(q);
        //p = deviceTransform(v->viewportTransform().inverted()).map(q);
        p = mapFromScene(v->viewportTransform().inverted().map(q));
        fDebug(DEBUG_PATHS) << "view.mapToScene:" << v->mapToScene(q.toPoint());
        fDebug(DEBUG_PATHS) << "this.mapFromScene(view.mapToScene):" << mapFromScene(v->mapToScene(q.toPoint()));
    }
    if (node == innerNode && innerNodeEdge)
    {
        node->setPos(p);
        node->setExtender(false);
        node->setAcceptsHoverEvents(true);
        scene()->clearSelection();
        node->setSelected(true);

        PathNode *m = innerNodeEdge->inNode;
        PathNode *n = innerNodeEdge->outNode;
        if (m->outEdge == n->inEdge && (m->outEdge != 0))
        {
            length -= m->outEdge->length();
            delete m->outEdge;
            addEdge(m,node);
            addEdge(node,n);
        }
        innerNode = newInnerNode(this);
        innerNodeEdge = 0;
        return;
    }
    node->setParentItem(this);
    node->setPos(p);
    node->setExtender(false);
    scene()->clearSelection();
    node->setSelected(true);

    if (node == tailExtenderNode)
    {
        addEdge(tail,node);
        tail = node;

        tailExtenderNode = newExtenderNode(&tailExtenderLine);

        tailExtenderLine.setPos(tail->pos());
        tailExtenderLine.setRotation(-tail->inEdge->angle2());
        tailExtenderLine.hide();
    }
    if (node == headExtenderNode)
    {
        addEdge(node,head);
        head = node;

        headExtenderNode = newExtenderNode(&headExtenderLine);

        headExtenderLine.setPos(head->pos());
        headExtenderLine.setRotation(180-head->outEdge->angle1());
        headExtenderLine.hide();
    }
}

void PathGraphicsItem::nodeReleased(PathNode *)
{
    PathNode *n = head;
    while (n)
    {
        PathNode *m = n->outNode;
        if (n->isSelected() && n->outEdge)
        {
            n->outEdge->updateSegments(false);
        }
        n = m;
    }
    updateExtenders();
}

void PathGraphicsItem::edgeHovered(PathEdge *e, const QPointF &pos)
{
    if (e)
    {
        innerNode->setPos(pos);
        innerNode->show();
    }
    else
    {
        innerNode->hide();
    }
    innerNodeEdge = e;
}

void PathGraphicsItem::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "PathGraphicsItem::keyPressEvent" << event;
    if (event->key() == Qt::Key_Delete)
    {
        PathNode *n = head;
        while (n)
        {
            PathNode *m = n->outNode;
            if (n->isSelected())
                removeNode(n);
            n = m;
        }
        updateExtenders();
    }
}
