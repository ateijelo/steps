/*
 * geocircle.h -- part of Steps, a simple maps app
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

#ifndef GEOCIRCLE_H
#define GEOCIRCLE_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>

class GeoCircle;

class GeoCircleNode : public QGraphicsEllipseItem
{
    public:
        GeoCircleNode(QGraphicsItem *parent = 0);
        void setParentCircle(GeoCircle *circle);
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    private:
        GeoCircle *parentCircle;
};

class GeoCircleLine : public QGraphicsLineItem
{
    public:
        GeoCircleLine(QGraphicsItem *parent = 0);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class GeoCircle : public QGraphicsItem
{
    public:
        GeoCircle(QGraphicsItem *parent = 0);
        void nodeMoved(GeoCircleNode *node);
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    private:
        void updateSegments();
        void addCardinalPoint(const QString& label);

        GeoCircleNode *center;
        GeoCircleNode *target;
        QList<GeoCircleLine*> segments;
        QList<QGraphicsTextItem*> cardinalLabels;
        QList<QGraphicsEllipseItem*> cardinalDots;
};

#endif // GEOCIRCLE_H
