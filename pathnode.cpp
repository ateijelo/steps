#include <cmath>
#include <QtDebug>
#include <QCursor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "pathnode.h"
#include "pathedge.h"
#include "pathgraphicsitem.h"
#include "geotools.h"

PathNode::PathNode(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), inEdge(0), outEdge(0),
      inNode(0), outNode(0), hovered(false)
{
    setFlag(ItemSendsScenePositionChanges);
    //setFlag(ItemIsFocusable);
    setFlag(ItemIsMovable);
    setFlag(ItemIgnoresTransformations);

    setAcceptHoverEvents(true);
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
//    qDebug() << "PathNode::mousePressEvent" << event;
//    qDebug() << "event->buttons():" << event->buttons();
    if (event->buttons() & Qt::RightButton)
    {
        event->ignore();
        return;
    }
//    qDebug() << "PathNode::mousePressEvent" << event;
    QGraphicsEllipseItem::mousePressEvent(event);

    if (event->button() == Qt::LeftButton && isExtender)
    {
        parentPath->extenderClicked(this);
    }
    if (event->button() == Qt::LeftButton && (event->modifiers() & Qt::ShiftModifier))
        parentPath->removeNode(this);
    if (event->button() == Qt::MiddleButton)
    {
        parentPath->removeNode(this);
    }
}

void PathNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseReleaseEvent(event);
//    qDebug() << "mouseReleaseEvent";

    if (event->button() == Qt::LeftButton)
    {
        parentPath->nodeReleased(this);
    }
}

void PathNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier)
    {
        parentPath->removeNode(this);
        event->accept();
        return;
    }
}

void PathNode::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    hovered = true;
    update();
}

void PathNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    hovered = false;
    update();
}

void PathNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
                setBrush(QBrush(Qt::red));
                width = 8;
            }
            else
            {
                setPen(QPen(QBrush(Qt::red),1.5));
                setBrush(QBrush());
                width = 8;
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
//    if (isExtender && hovered)
//    {
//        setPen
//        painter->setPen(QPen(QBrush(Qt::red),1));
//        painter->setBrush(QBrush(Qt::red));
//        painter->drawEllipse(-width/2+1,-width/2+1,width-2,width-2);
//    }
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawEllipse(-width/2,-width/2,width,width);
//    QLineF l(0,0,width/2+pen().width(),0);
//    painter->setPen(QPen(QBrush(Qt::red),3,Qt::SolidLine,Qt::FlatCap));
//    if (inEdge)
//    {
//        QTransform t;
//        t.rotate(180-inEdge->angle2());
//        painter->drawLine(t.map(l));
//    }
//    if (outEdge)
//    {
//        QTransform t;
//        t.rotate(-outEdge->angle1());
//        painter->drawLine(t.map(l));
//    }
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
        case ItemPositionChange:
            {
                QPointF p = value.toPointF();
//                qDebug() << "itemChange" << p;
                qreal pw = GeoTools::projectionWidth();
//                qDebug() << "pw:" << pw;
                qreal x = p.x();
                if (x > -pw/2 && x < pw/2)
                    return value;
                qreal m = fmod(p.x() + pw/2,pw);
                if (m < 0)
                    p.setX(pw/2 + m);
                else
                    p.setX(-pw/2 + m);
                return p;
            }
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
