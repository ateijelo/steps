/*
 * pathnode.h -- part of Steps, a simple maps app
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

#ifndef PATHNODE_H
#define PATHNODE_H

#include <QGraphicsEllipseItem>

#include "pathnodegraphicsitem.h"

class PathGraphicsItem;
class PathEdge;
class Path;

class PathNode
{
    public:
        PathNode(Path *path, const QList<PathGraphicsItem *> &parents);
        PathNode(Path *path, const QList<QGraphicsLineItem *> &parents);
        ~PathNode();

        void setParents(const QList<PathGraphicsItem *> &parents);

        QPointF pos() const;
        void setPos(const QPointF& p);
        void setPos(qreal x, qreal y);
        void show();
        void hide();

        bool hovered() const;
        void setHovered(bool h);
        bool isExtender() const;
        void setExtender(bool b);
        void setAcceptHoverEvents(bool b);

        void moved(PathNodeGraphicsItem* item);
        void selectedChanged(PathNodeGraphicsItem* item, bool selected);
        void setSelected(bool b = true);
        bool isSelected() const;

        void clicked(Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
        QTransform deviceTransform(const QTransform& viewportTransform);

        // parent or sibling objects, not my responsibility to delete
        Path *path;
        PathEdge *inEdge = nullptr;
        PathEdge *outEdge = nullptr;
        PathNode *inNode = nullptr;
        PathNode *outNode = nullptr;

    private:
        bool _isExtender = false;
        bool _hovered = false;
        bool _selected = false;
        bool updates_enabled = true;
        // child objects, it's my responsibility to delete them
        QList<PathNodeGraphicsItem*> items;

};

#endif // PATHNODE_H
