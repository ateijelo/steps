/*
 * pathedgesegment.cpp -- part of Steps, a simple maps app
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

#include "pathgraphicsitem.h"
#include "pathedgesegment.h"
#include "pathedgesegmentgraphicsitem.h"

PathEdgeSegment::PathEdgeSegment(PathEdge *edge)
    : edge(edge)
{
    color = Qt::red;
    randomColor.setHsv(qrand() % 256,255,255);

    for (auto& p: edge->pathGraphicsItems()) {
        items.append(new PathEdgeSegmentGraphicsItem(this, p));
    }
    //hoverNode = new QGraphicsEllipseItem(this);
    //hoverNode->setRect(-5,-5,10,10);
    //hoverNode->setFlag(ItemIgnoresTransformations);
    //hoverNode->setFlag(ItemStacksBehindParent);
    //hoverNode->setPen(QPen(QBrush(Qt::yellow),1.5));
    //hoverNode->hide();
}

PathEdgeSegment::~PathEdgeSegment()
{
    foreach (PathEdgeSegmentGraphicsItem *i, items) {
        delete i;
    }
}

qreal PathEdgeSegment::angle()
{
    return items.at(0)->line().angle();
}

void PathEdgeSegment::hoverMove(const QPointF &p) const
{
    edge->hoverMove(p);
}

void PathEdgeSegment::hoverLeave() const
{
    edge->hoverLeave();
}

void PathEdgeSegment::setLine(const QLineF &l)
{
    for (auto& i: items) {
        i->setLine(l);
    }
}

//void PathEdgeSegment::setParentEdge(PathEdge *edge)
//{
//    parentEdge = edge;
//}


