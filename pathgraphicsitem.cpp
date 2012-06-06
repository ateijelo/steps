//#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QPen>
#include <QBrush>
#include <cstdlib>
#include <QCursor>
#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <GeographicLib/GeodesicLine.hpp>

#include "pathgraphicsitem.h"
#include "geotools.h"

PathGraphicsItem::PathGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent), head(0), tail(0), length(0.0),
      tailExtenderLine(this), headExtenderLine(this)
{
//    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(ItemHasNoContents);
    this->setFlag(ItemIsFocusable);
    for (int i=0; i<2; i++)
        addNode(QPointF(i*1000000,0));

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
    e->setParentPath(this);
    e->setZValue(1);
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
    qDebug() << "    length: " << length;
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
    qDebug() << "    length: " << length;
    if (node == tail)
    {
        qDebug() << "tail node moved";
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
}

void PathGraphicsItem::removeNode(PathNode *node)
{
    if (head == tail)
        return;
    if (head->outNode == tail)
        return;
    if (tail->inNode == head)
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
        QPointF q = node->deviceTransform(v->viewportTransform()).map(QPointF(0,0));
        p = deviceTransform(v->viewportTransform().inverted()).map(q);
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

void PathGraphicsItem::nodeReleased(PathNode *node)
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

// ---------------- PathNode ----------------

PathNode::PathNode(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), inEdge(0), outEdge(0),
      inNode(0), outNode(0), hovered(false)
{
    setFlag(ItemSendsScenePositionChanges);
    //setFlag(ItemIsFocusable);
    setFlag(ItemIsMovable);
    setFlag(ItemIgnoresTransformations);

    setAcceptsHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    setExtender(false);
    setRect(-10,-10,20,20);
}

void PathNode::setParentPath(PathGraphicsItem *path)
{
    parentPath = path;
    //setFocusProxy(path);
}

void PathNode::setExtender(bool b)
{
    isExtender = b;
    if (isExtender)
    {
        setFlag(ItemIsSelectable,false);
    }
    else
    {
        setFlag(ItemIsSelectable);
    }
}

void PathNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);

    if (event->button() == Qt::LeftButton && isExtender)
    {
        parentPath->extenderClicked(this);
    }
    if (event->button() == Qt::MiddleButton)
    {
        parentPath->removeNode(this);
    }
}

void PathNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseReleaseEvent(event);
    qDebug() << "mouseReleaseEvent";

    if (event->button() == Qt::LeftButton)
    {
        parentPath->nodeReleased(this);
    }
}

void PathNode::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hovered = true;
    update();
}

void PathNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hovered = false;
    update();
}

void PathNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width;

    if (isSelected())
    {
        setPen(QPen(QBrush(Qt::yellow),3));
        setBrush(Qt::red);
        width = 10;
    }
    else
    {
        if (isExtender)
        {
            if (hovered)
            {
                setPen(QPen(QBrush(Qt::yellow),1.5));
                setBrush(QBrush());
                width = 10;
            }
            else
            {
                setPen(QPen(QBrush(Qt::red),1.5));
                setBrush(QBrush());
                width = 10;
            }
        }
        else
        {
            if (hovered)
            {
                setPen(QPen(QBrush(Qt::yellow),2));
                setBrush(Qt::red);
                width = 8;
            }
            else
            {
                setPen(QPen(QBrush(Qt::red),3));
                setBrush(Qt::red);
                width = 6.5;
            }
        }
    }
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawEllipse(-width/2,-width/2,width,width);
}

QPainterPath PathNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10,-10,20,20);
    return path;
}

QVariant PathNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
        case ItemScenePositionHasChanged:
            parentPath->nodeMoved(this);
            return QVariant();
            break;
        case ItemSelectedHasChanged:
            {
                if (value.toBool())
                    parentPath->setFocus();
                parentPath->nodeSelectedChanged(this,value.toBool());
                return QVariant();
            }
            break;
        default:
            return value;
    }
}

// ---------------- PathEdge ----------------

PathEdge::PathEdge(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent)
    : QGraphicsItem(parent), p1(p1), p2(p2)
{
    setFlag(ItemHasNoContents);
    updateSegments();
}

void PathEdge::setParentPath(PathGraphicsItem *path)
{
    parentPath = path;
}

QRectF PathEdge::boundingRect() const
{
    return QRectF();
}

void PathEdge::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void PathEdge::setP1(const QPointF &p, bool fast)
{
    p1 = p;
    updateSegments(fast);
}

void PathEdge::setP2(const QPointF &p, bool fast)
{
    p2 = p;
    updateSegments(fast);
}

qreal PathEdge::angle1() const
{
    return segments.first()->line().angle();
}

qreal PathEdge::angle2() const
{
    return segments.last()->line().angle();
}

void PathEdge::subdivide(QLinkedList<QPointF>& points, QLinkedList<QPointF>::iterator i,
                         double lat1, double lon1, double azi1, double s1, double s2, int depth)
{
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84;
    if (depth > 4)
        return;
    if (fastUpdate)
        return;
    QPointF p = *i;
    QPointF r = *(i+1);
    double lat2,lon2;
    //double s12 = s2 - s1;
    double d = 0;
    //double d = (((double)qrand()/RAND_MAX)*0.2 - 0.1)*s12;
    double sd = (s1+s2)/2+d;
    //qDebug() << s1 << sd << s2;
    g.Direct(lat1,lon1,azi1,sd,lat2,lon2);
    QPointF q(GeoTools::LatLon2Meters(QPointF(lon2,lat2)));

    double ax = q.x()-p.x();
    double ay = q.y()-p.y();
    double d2a = ax*ax + ay*ay;
    double bx = r.x()-q.x();
    double by = r.y()-q.y();
    double d2b = bx*bx + by*by;
    double adotb = ax*bx + ay*by;
    double cos2 = adotb * qAbs(adotb) / (d2a * d2b);

    if (cos2 < 0.9995)
    {
        points.insert(i+1,q);
        subdivide(points,i+1,lat1,lon1,azi1,sd,s2,depth+1);
        subdivide(points,i,lat1,lon1,azi1,s1,sd,depth+1);
    }
}

void PathEdge::updateSegments(bool fast)
{
    fastUpdate = fast;
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84;
    GeoTools gt;
    QPointF q1 = mapToScene(p1);
    QPointF q2 = mapToScene(p2);
    QPointF r1 = gt.Meters2LatLon(q1);
    QPointF r2 = gt.Meters2LatLon(q2);
    double azi1, azi2, s12;
    g.Inverse(r1.y(),r1.x(),r2.y(),r2.x(),s12,azi1,azi2);

    QLinkedList<QPointF> points;
    points.append(q1);
    points.append(q2);
    subdivide(points,points.begin(),r1.y(),r1.x(),azi1,0,s12,0);

    QLinkedList<PathEdgeSegment*>::iterator si = segments.begin();
    QLinkedList<QPointF>::iterator i = points.begin();
    while (true)
    {
        if (i == points.end())
            break;
        if ((i+1) == points.end())
            break;
        PathEdgeSegment *s;
        if (si != segments.end())
        {
            s = *si;
            si++;
        }
        else
        {
            s = new PathEdgeSegment(this);
            segments.append(s);
        }
        //qDebug() << GeoTools::Meters2LatLon(*i) << "->" << GeoTools::Meters2LatLon(*(i+1));
        QPointF t1 = mapFromScene(*i);
        QPointF t2 = mapFromScene(*(i+1));
        double w = GeoTools::projectionWidth();
        if (qAbs(t2.x() - w - t1.x()) < qAbs(t2.x() - t1.x()))
            s->setLine(QLineF(t1,t2 - QPointF(w,0)));
        else if (qAbs(t2.x() + w - t1.x()) < qAbs(t2.x() - t1.x()))
            s->setLine(QLineF(t1,t2 + QPointF(w,0)));
        else
            s->setLine(QLineF(t1,t2));
        i++;
    }
    while (si != segments.end())
    {
        delete *si;
        si = segments.erase(si);
    }
    qDebug() << "segments.size:" << segments.size();
}

double PathEdge::length() const
{
    QPointF q1 = mapToScene(p1);
    QPointF q2 = mapToScene(p2);
    GeoTools gt;
    q1 = gt.Meters2LatLon(p1);
    q2 = gt.Meters2LatLon(p2);
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84;
    double azi1, azi2,s12;
    g.Inverse(q1.y(),q1.x(),q2.y(),q2.x(),s12,azi1,azi2);

    return s12;
}

// ---------------- PathEdge ----------------

PathEdgeSegment::PathEdgeSegment(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    c.setHsv(qrand() % 256,255,255);

    hoverNode = new QGraphicsEllipseItem(this);
    hoverNode->setRect(-5,-5,10,10);
    hoverNode->setFlag(ItemIgnoresTransformations);
    //hoverNode->setFlag(ItemStacksBehindParent);
    hoverNode->setPen(QPen(QBrush(Qt::yellow),1.5));
    hoverNode->hide();
}

void PathEdgeSegment::setParentPath(PathGraphicsItem *path)
{
    parentPath = path;
}

void PathEdgeSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 20;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setPen(QPen(QBrush(QColor(0,0,0,0)),width/lod,Qt::SolidLine,Qt::RoundCap));

    QGraphicsLineItem::paint(painter,option,widget);
    painter->setPen(QPen(QBrush(c),3/lod,Qt::SolidLine,Qt::RoundCap));
    painter->drawLine(line());
}

void PathEdgeSegment::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        QLineF l = line();
        QLineF m = l.normalVector();
        m.translate(event->pos()-m.p1());
        QPointF p;
        l.intersect(m,&p);
        hoverNode->setPos(p);
        hoverNode->show();
    }
    else
    {
        hoverNode->hide();
    }
}

void PathEdgeSegment::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hoverNode->hide();
}
