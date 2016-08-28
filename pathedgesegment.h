/*
 * pathedgesegment.h -- part of Steps, a simple maps app
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

#ifndef PATHEDGESEGMENT_H
#define PATHEDGESEGMENT_H

#include "path.h"
#include "pathedge.h"

class PathEdgeSegmentGraphicsItem;

class PathEdgeSegment
{
    public:
        PathEdgeSegment(PathEdge *edge);
        ~PathEdgeSegment();
        QColor color;
        QColor randomColor;
        qreal angle();
        void hoverMove(const QPointF& p) const;
        void hoverLeave() const;
        void setLine(const QLineF &l);

    private:
        PathEdge *edge;
        QList<PathEdgeSegmentGraphicsItem*> items;
};


#endif // PATHEDGESEGMENT_H
