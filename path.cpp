#include "debug.h"

#include "geotools.h"
#include "path.h"

Path::Path(const QPointF &from, const QPointF &to, QObject *parent) :
    QObject(parent), _length(0)
{

    for (auto i: {1,2,3}) {
        auto item = new PathGraphicsItem(this);
        items.append(item);
    }

    head = new PathNode(this);
    tail = new PathNode(this);

    head->setPos(to);
    tail->setPos(from);

    addEdge(head, tail);

    updateExtenderLines();

    createHoverNode();

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

PathNode *Path::newExtenderNode(QGraphicsItem *parent)
{
    PathNode *n = new PathNode(this);
    n->setExtender(true);
    n->setPos(40,0);
    return n;
}

void Path::createHoverNode()
{
    hoverNode = new PathNode(this);
    hoverNode->setAcceptHoverEvents(false);
    hoverNode->setExtender(true);
    hoverNode->setHovered(true);
    hoverNode->hide();
}

void Path::updateExtenderLines()
{
    for (auto& i: items) {
//        i->updateTail(tail->pos(), -tail->inEdge->angle2());
//        i->updateHead(head->pos(), 180-head->outEdge->angle1());
    }
}
