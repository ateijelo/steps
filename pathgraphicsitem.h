/*
 * pathgraphicsitem.h -- part of Steps, a simple maps app
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

#ifndef PATHGRAPHICSITEM_H
#define PATHGRAPHICSITEM_H

#include <QGraphicsItem>

class Path;
class PathNode;
class PathEdge;

class PathGraphicsItem : public QGraphicsItem
{
    public:
        explicit PathGraphicsItem(Path *path, QGraphicsItem *parent = 0);
        ~PathGraphicsItem();
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
        void keyPressEvent(QKeyEvent *event);
//        void addEdge(PathNode *from, PathNode *to);
//        void nodeMoved(PathNode *node);
//        void nodeSelectedChanged(PathNode *node, bool selected);
//        void removeNode(PathNode *node);
//        void setPos(const QPointF &pos);
//        void extenderClicked(PathNode *node);
//        void nodeReleased(PathNode *);
//        void edgeHovered(PathEdge *e, const QPointF& pos);
//        void addSibling(PathGraphicsItem *p);
//        double length();
//        void updateTailExtender(const QPointF& pos, qreal angle);
//        void updateHeadExtender(const QPointF& pos, qreal angle);
//        void showTailExtender(bool b = true);
//        void showHeadExtender(bool b = true);

    signals:

    public slots:

    private:
//        PathNode *newExtenderNode(QGraphicsItem *parent);
//        PathNode *newInnerNode();
//        void updateExtenders();
//        void addNode(const QPointF& pos);

        Path *path;
//        QGraphicsLineItem tailExtenderLine;
//        QGraphicsLineItem headExtenderLine;
};

#endif // PATHGRAPHICSITEM_H
