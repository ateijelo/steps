//#include <cmath>
#include <QtDebug>
#include <QCursor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "geotools.h"
#include "pathnode.h"
#include "pathnodegraphicsitem.h"

PathNodeGraphicsItem::PathNodeGraphicsItem(PathNode *node, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), node(node)
{
    setFlag(ItemSendsScenePositionChanges);
    //setFlag(ItemIsFocusable);
    setFlag(ItemIsMovable);
    setFlag(ItemIgnoresTransformations);

    setZValue(2);
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
//    setExtender(false);
    setRect(-10,-10,20,20);
}

void PathNodeGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "PathNodeGraphicsItem::mousePressEvent" << event;
//    qDebug() << "event->buttons():" << event->buttons();
    if (event->buttons() & Qt::RightButton)
    {
        event->ignore();
        return;
    }
//    qDebug() << "PathNodeGraphicsItem::mousePressEvent" << event;
    QGraphicsEllipseItem::mousePressEvent(event);

    node->clicked(event->button(), event->modifiers());
}

void PathNodeGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseReleaseEvent(event);
//    qDebug() << "mouseReleaseEvent";

    if (event->button() == Qt::LeftButton)
    {
        //path->nodeReleased(this);
    }
}

void PathNodeGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier)
    {
        //path->removeNode(this);
        event->accept();
        return;
    }
}

void PathNodeGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    node->setHovered(true);
}

void PathNodeGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    node->setHovered(false);
}

void PathNodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    qreal width;

    if (node->isSelected())
    {
        setPen(QPen(QBrush(Qt::yellow),3));
        setBrush(Qt::red);
        width = 10;
    }
    else
    {
        if (node->isExtender())
        {
            if (node->hovered())
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
            if (node->hovered())
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

QPainterPath PathNodeGraphicsItem::shape() const
{
    QPainterPath path;
    path.addEllipse(-10,-10,20,20);
    return path;
}

QVariant PathNodeGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
        case ItemScenePositionHasChanged:
            node->moved(this);
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
                qDebug() << "ItemSelectedHasChanged(" << scenePos() << "," << value.toBool() << ")";
                if (value.toBool())
                    parentItem()->setFocus();
                node->selectedChanged(this,value.toBool());
                update();
                return QVariant();
            }
            break;
        default:
            return value;
    }
}
