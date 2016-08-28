/*
 * geocircle.cpp -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtDebug>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <GeographicLib/GeodesicLine.hpp>

#include "geocircle.h"
#include "geotools.h"

GeoCircleNode::GeoCircleNode(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsScenePositionChanges);
    setFlag(ItemIgnoresTransformations);
    setFlag(ItemIsSelectable);
    setCursor(Qt::ArrowCursor);
    setRect(-5,-5,10,10);
    setBrush(Qt::white);
    setPen(QPen(QBrush(Qt::black),3));
}

void GeoCircleNode::setParentCircle(GeoCircle *circle)
{
    parentCircle = circle;
}

GeoCircleLine::GeoCircleLine(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
}

void GeoCircleLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal width = 3;
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    setPen(QPen(QBrush(Qt::black),width/lod,Qt::SolidLine,Qt::RoundCap));

    QGraphicsLineItem::paint(painter,option,widget);
}

QVariant GeoCircleNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
        case ItemScenePositionHasChanged:
            parentCircle->nodeMoved(this);
            return QVariant();
            break;
        default:
            return value;
    }
}

GeoCircle::GeoCircle(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    center = new GeoCircleNode(this);
    target = new GeoCircleNode(this);

    setFlag(ItemHasNoContents);
    for (int i=0; i<60; i++)
    {
        GeoCircleLine *l = new GeoCircleLine(this);
        l->setZValue(1);
        segments.append(l);
    }
    center->setParentCircle(this);
    center->setZValue(2);
    center->setBrush(Qt::blue);
    target->setParentCircle(this);
    target->setZValue(2);

    const char *labels[] = {"N","E","S","W"};
    for (int i=0; i<4; i++)
    {
        addCardinalPoint(labels[i]);
    }

    target->setPos(1000000,0);
    center->setPos(0,0);
    updateSegments();
}

void GeoCircle::addCardinalPoint(const QString &label)
{
    QGraphicsTextItem *t = new QGraphicsTextItem(label,this);
    t->setFlag(ItemIgnoresTransformations);
    cardinalLabels.append(t);
    QGraphicsEllipseItem *e = new QGraphicsEllipseItem(this);
    e->setFlag(ItemIgnoresTransformations);
    e->setRect(-3,-3,6,6);
    e->setBrush(Qt::black);
    cardinalDots.append(e);
}

void GeoCircle::nodeMoved(GeoCircleNode *node)
{
    Q_UNUSED(node);
    updateSegments();
}

QRectF GeoCircle::boundingRect() const
{
    return QRectF();
}

void GeoCircle::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void GeoCircle::updateSegments()
{
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();
    QPointF c = GeoTools::Meters2LatLon(center->scenePos());
    QPointF t = GeoTools::Meters2LatLon(target->scenePos());

    double s12;
    double azi1,azi2;
    g.Inverse(c.y(),c.x(),t.y(),t.x(),s12,azi1,azi2);
    qDebug() << "GeoCircle radius:" << s12;

    double lat1,lon1;
    double lat2,lon2;
    g.Direct(c.y(),c.x(),azi1,s12,lat1,lon1);
    QPointF p = GeoTools::LatLon2Meters(QPointF(lon1,lat1));
    for (int i=0; i<segments.size(); i++)
    {
        g.Direct(c.y(),c.x(),azi1+(i+1)*(360.0/segments.size()),s12,lat2,lon2);
        QPointF q = GeoTools::LatLon2Meters(QPointF(lon2,lat2));
        segments.at(i)->setLine(QLineF(mapFromScene(p),mapFromScene(q)));
        if (qAbs(p.x() - q.x()) > (GeoTools::projectionWidth()/2))
            segments.at(i)->hide();
        else
            segments.at(i)->show();
        p = q;
    }

    for (int i=0; i<4; i++)
    {
        g.Direct(c.y(),c.x(),90*i,s12,lat1,lon1);
        cardinalLabels.at(i)->setPos(mapFromScene(GeoTools::LatLon2Meters(QPointF(lon1,lat1))));
        cardinalDots.at(i)->setPos(mapFromScene(GeoTools::LatLon2Meters(QPointF(lon1,lat1))));
    }
}
