/*
 * pathnodegraphicsitem.h -- part of Steps, a simple maps app
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

#ifndef PATHNODEGRAPHICSITEM_H
#define PATHNODEGRAPHICSITEM_H

#include <QGraphicsEllipseItem>

class PathNode;

class PathNodeGraphicsItem : public QGraphicsEllipseItem
{
    public:
        PathNodeGraphicsItem(PathNode *node, QGraphicsItem *parent);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
        void hoverEnterEvent(QGraphicsSceneHoverEvent *);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QPainterPath shape() const;
        QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

        void setHovered(bool hovered);

        PathNode *node;
//        PathEdge *inEdge;
//        PathEdge *outEdge;
//        PathNode *inNode;
//        PathNode *outNode;
//        bool isExtender;
};


#endif // PATHNODEGRAPHICSITEM_H
