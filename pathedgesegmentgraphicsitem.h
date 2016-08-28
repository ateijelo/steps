/*
 * pathedgesegmentgraphicsitem.h -- part of Steps, a simple maps app
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

#ifndef PATHEDGESEGMENTGRAPHICSITEM_H
#define PATHEDGESEGMENTGRAPHICSITEM_H

#include <QGraphicsItem>

#include "pathedgesegment.h"

class PathEdgeSegmentGraphicsItem : public QGraphicsLineItem
{
    public:
        PathEdgeSegmentGraphicsItem(PathEdgeSegment *segment, QGraphicsItem *parent = nullptr);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    private:
        QGraphicsEllipseItem *hoverNode;
        PathEdgeSegment *segment;

};

#endif // PATHEDGESEGMENTGRAPHICSITEM_H
