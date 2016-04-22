#include <QGraphicsView>

#include "debug.h"

#include "geotools.h"
#include "path.h"

Path::Path(const QPointF &from, const QPointF &to, QObject *parent) :
    QObject(parent), _length(0)
{
    for (auto i: {1,2,3}) {
        auto item = new PathGraphicsItem(this);
        items.append(item);

        auto tl = new QGraphicsLineItem(item);
        tl->setLine(0,0,35,0);
        tl->setFlag(tl->ItemIgnoresTransformations);
        tl->setPen(QPen(QBrush(Qt::red),1.5,Qt::SolidLine,Qt::RoundCap));
        tailExtenderLines.append(tl);

        auto hl = new QGraphicsLineItem(item);
        hl->setLine(0,0,35,0);
        hl->setFlag(tl->ItemIgnoresTransformations);
        hl->setPen(QPen(QBrush(Qt::red),1.5,Qt::SolidLine,Qt::RoundCap));
        headExtenderLines.append(hl);
    }

    head = new PathNode(this, items);
    tail = new PathNode(this, items);
    head->setPos(to);
    tail->setPos(from);
    addEdge(head, tail);

    hoverNode = newHoverNode();

    headExtenderNode = newExtenderNode(headExtenderLines);
    tailExtenderNode = newExtenderNode(tailExtenderLines);
    updateExtenderLines();

    _name = "";
    _visible = true;
}

void Path::addEdge(PathNode *from, PathNode *to)
{
    PathEdge *e = new PathEdge(from->pos(),to->pos(),this);
    e->inNode = from;
    e->outNode = to;
    from->outEdge = e;
    to->inEdge = e;
    from->outNode = to;
    to->inNode = from;
    _length += e->length();
}

void Path::extenderClicked(PathNode *node)
{
    QPointF p;
    QList<QGraphicsView*> l = items.at(0)->scene()->views();
    if (l.size() > 0)
    {
        // tail & head extenders are children of their respective extender lines
        // this maps their coords to scene coords, so that when reparented to
        // the PathGraphicsItem the fall in the right position

        QGraphicsView *v = l.at(0);
        fDebug(DEBUG_PATHS) << "viewportTransform:" << v->viewportTransform();
        fDebug(DEBUG_PATHS) << "node->deviceTransform:" << node->deviceTransform(v->viewportTransform());
        fDebug(DEBUG_PATHS) << "  .map:" << node->deviceTransform(v->viewportTransform()).map(QPointF(0,0));
        QPointF q = node->deviceTransform(v->viewportTransform()).map(QPointF(0,0));
        fDebug(DEBUG_PATHS) << "  deviceTransform.map:" << items.at(0)->deviceTransform(v->viewportTransform().inverted()).map(q);
        //p = deviceTransform(v->viewportTransform().inverted()).map(q);
        p = items.at(0)->mapFromScene(v->viewportTransform().inverted().map(q));
        fDebug(DEBUG_PATHS) << "view.mapToScene:" << v->mapToScene(q.toPoint());
        fDebug(DEBUG_PATHS) << "this.mapFromScene(view.mapToScene):" << items.at(0)->mapFromScene(v->mapToScene(q.toPoint()));
    }

    node->setExtender(false);
    items.at(0)->scene()->clearSelection();
    node->setSelected(true);
    node->setPos(p);
    if (node == hoverNode && hoverEdge != nullptr)
    {
        node->setAcceptHoverEvents(true);

        PathNode *m = hoverEdge->inNode;
        PathNode *n = hoverEdge->outNode;
        if (m->outEdge == n->inEdge && (m->outEdge != 0))
        {
            _length -= m->outEdge->length();
            delete m->outEdge;
            addEdge(m,node);
            addEdge(node,n);
        }
        hoverNode = newHoverNode();
        hoverEdge = nullptr;
        return;
    }
    //node->setParentItem(this);
    //node->setPos(p);

    if (node == tailExtenderNode)
    {
        addEdge(tail,node);
        node->setParents(items);
        tail = node;

        tailExtenderNode = newExtenderNode(tailExtenderLines);
//        tailExtenderLine.setPos(tail->pos());
//        tailExtenderLine.setRotation(-tail->inEdge->angle2());
//        tailExtenderLine.hide();
    }
    if (node == headExtenderNode)
    {
        addEdge(node,head);
        node->setParents(items);
        head = node;
        headExtenderNode = newExtenderNode(headExtenderLines);
//        headExtenderLine.setPos(head->pos());
//        headExtenderLine.setRotation(180-head->outEdge->angle1());
//        headExtenderLine.hide();
    }
    updateExtenderLines();
}

void Path::addToScene(QGraphicsScene *scene)
{
    auto w = GeoTools::projectionWidth();

    qreal offsets[] = {0, -w, w};

    int j = 0;
    for (auto& i: items) {
        i->setZValue(2);
        i->setPos(QPointF(offsets[j], 0)); j++;
        scene->addItem(i);
    }
}

void Path::setName(const QString &name)
{
    if (_name != name) {
        _name = name;
        emit nameChanged(name);
    }
}

void Path::setVisible(bool visible)
{
    if (_visible != visible) {
        _visible = visible;
        emit visibleChanged(visible);
    }
}

void Path::setLength(double l)
{
    if (_length != l) {
        _length = l;
        emit lengthChanged(l);
    }
}

void Path::nodeMoved(PathNode *node)
{
    if (node->inEdge)
    {
        _length -= node->inEdge->length();
        bool fast = false;
        if (node->inNode->isSelected())
            fast = true;
        node->inEdge->setP2(node->pos(),fast);
        _length += node->inEdge->length();
    }
    if (node->outEdge)
    {
        _length -= node->outEdge->length();
        bool fast = false;
        if (node->outNode->isSelected())
            fast = true;
        node->outEdge->setP1(node->pos(),fast);
        _length += node->outEdge->length();
    }
    fDebug(DEBUG_PATHS) << "    length: " << _length;
    //lengthLabel->setText(QString("%1 meters").arg(length,0,'f',2));
    setLength(_length);
//    if (node == tail)
//    {
//        tailExtenderLine.setPos(tail->pos());
//        tailExtenderLine.setRotation(-tail->inEdge->angle2());
//    }
//    if (node->outNode == tail)
//        tailExtenderLine.setRotation(-tail->inEdge->angle2());
//    if (node == head)
//    {
//        headExtenderLine.setPos(head->pos());
//        headExtenderLine.setRotation(180-head->outEdge->angle1());
//    }
//    if (node->inNode == head)
//        headExtenderLine.setRotation(180-head->outEdge->angle1());
    updateExtenderLines();
}

void Path::nodeSelectedChanged(PathNode *node, bool selected)
{
    updateExtenderLines();
//    if (node == tail)
//        tailExtenderLine.setVisible(selected);
//    if (node == head)
//        headExtenderLine.setVisible(selected);
    //    fDebug(DEBUG_PATHS) << "    length:" << _length;
    //    //lengthLabel->setText(QString("%1 meters").arg(length,0,'f',2));
}

void Path::edgeHovered(PathEdge *e, const QPointF &pos)
{
    if (e)
    {
        hoverNode->setPos(pos);
        hoverNode->show();
    }
    else
    {
        hoverNode->hide();
    }
    hoverEdge = e;
}

//QPointF Path::headPos() const { return head->pos(); }

//QPointF Path::tailPos() const { return tail->pos(); }

//qreal Path::headAngle() const
//{
//    //180-head->outEdge->angle1()
//    return 0;
//}

//qreal Path::tailAngle() const
//{
//    // -tail->inEdge->angle2()
//    return 0;
//}

QPointF Path::mapToScene(const QPointF &p)
{
    return items.at(0)->mapToScene(p);
}

QPointF Path::mapFromScene(const QPointF &p)
{
    return items.at(0)->mapFromScene(p);
}

PathNode *Path::newExtenderNode(const QList<QGraphicsLineItem *> &parents)
{
    PathNode *n = new PathNode(this, parents);
    n->setExtender(true);
    n->setPos(40,0);
    return n;
}

PathNode* Path::newHoverNode()
{
    PathNode *n = new PathNode(this, items);
    n->setAcceptHoverEvents(false);
    n->setExtender(true);
    n->setHovered(true);
    n->hide();
    return n;
}

void Path::updateExtenderLines()
{
//    qDebug() << "tail:" << tail;
//    qDebug() << "tail->pos():" << tail->pos();
//    qDebug() << "tailExtenderLines.at(0):" << tailExtenderLines.at(0);
//    QPointF p = tail->pos();
//    tailExtenderLines.at(0)->setPos(p.x(), p.y());
//    tailExtenderLines.at(0)->setVisible(true);
    foreach (QGraphicsLineItem* l, tailExtenderLines) {
        l->setPos(tail->pos());
        l->setRotation(-tail->inEdge->angle2());
        l->setVisible(tail->isSelected());
    }
    foreach (QGraphicsLineItem* l, headExtenderLines) {
        l->setPos(head->pos());
        l->setRotation(180-head->outEdge->angle1());
        l->setVisible(head->isSelected());
    }
}
