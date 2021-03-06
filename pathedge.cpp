/*
 * pathedge.cpp -- part of Steps, a simple maps app
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
#include <GeographicLib/GeodesicLine.hpp>

#include "path.h"
#include "pathedge.h"
#include "pathedgesegment.h"
#include "pathgraphicsitem.h"
#include "geotools.h"

PathEdge::PathEdge(const QPointF &p1, const QPointF &p2, Path *path)
    : path(path), p1(p1), p2(p2)
{
    //setFlag(ItemHasNoContents);
    updateSegments();
}

PathEdge::~PathEdge()
{
    foreach (PathEdgeSegment *s, segments) {
        delete s;
    }
}

//void PathEdge::setParentPath(PathGraphicsItem *path)
//{
//    parentPath = path;
//    foreach (PathEdgeSegment *s, segments)
//        s->setParentPath(path);
//}

//QRectF PathEdge::boundingRect() const
//{
//    return QRectF();
//}

//void PathEdge::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
//{
//}

void PathEdge::setP1(const QPointF &p, bool fast)
{
    p1 = p;
    updateSegments(fast);
}

void PathEdge::setP2(const QPointF &p, bool fast)
{
    p2 = p;
    updateSegments(fast);
}

qreal PathEdge::angle1() const
{
    return segments.first()->angle();
}

qreal PathEdge::angle2() const
{
    return segments.last()->angle();
}

void PathEdge::hoverMove(const QPointF &p)
{
    path->edgeHovered(this, p);
}

void PathEdge::hoverLeave()
{
    path->edgeHovered(nullptr, QPointF());
}

void PathEdge::subdivide(QLinkedList<QPointF>& points, QLinkedList<QPointF>::iterator i,
                         double lat1, double lon1, double azi1, double s1, double s2, int depth)
{
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();

    if (depth >= 3)
        return;
    if (fastUpdate)
        return;
    if ((s2 - s1) < 200000)
        return;

//    QPointF p = *i;
//    QPointF r = *(i+1);

    double lat2,lon2;
    //double s12 = s2 - s1;
    //double d = 0;
    //double sd = (s1+s2)/2+d;

    int subpoints = 3;
    double step = 1.0 / (subpoints + 1);

    std::vector<QPointF> qs;
    qs.reserve(subpoints + 2);
    bool subdiv[subpoints+1];

    qs[0] = *i;
    qs[subpoints+1] = *(i+1);
    for (int k=0; k<subpoints; k++)
    {
        double sd = s1 + ((k+1)*step) * (s2 - s1);
        //sd += (((double)qrand()/RAND_MAX)*0.2 - 0.1) * (s2 - s1);
        g.Direct(lat1,lon1,azi1,sd,lat2,lon2);
        qs[k+1] = GeoTools::LatLon2Meters(QPointF(lon2,lat2));
    }

    for (int k=1; k<=subpoints; k++)
    {
        QPointF p = qs[k-1];
        QPointF q = qs[k];
        QPointF r = qs[k+1];

        double ax = q.x()-p.x();
        double ay = q.y()-p.y();
        double d2a = ax*ax + ay*ay;
        double bx = r.x()-q.x();
        double by = r.y()-q.y();
        double d2b = bx*bx + by*by;
        double adotb = ax*bx + ay*by;
        double cos2 = adotb * qAbs(adotb) / (d2a * d2b);

        if (cos2 < (1 - 0.001))
        {
            subdiv[k-1] = true;
            subdiv[k] = true;
        }
    }

    for (int k=subpoints; k>=0; k--)
    {
        if (!subdiv[k]) continue;
        double sa = s1 + ((k)*step) * (s2 - s1);
        double sb = s1 + ((k+1)*step) * (s2 - s1);
        if (k > 0)
        {
            points.insert(i+1,qs[k]);
            subdivide(points,i+1,lat1,lon1,azi1,sa,sb,depth+1);
        } else {
            subdivide(points,i,lat1,lon1,azi1,sa,sb,depth+1);
        }
    //            subdivide(points,i+1,lat1,lon1,azi1,sd,s2,depth+1);
    }

    //qDebug() << s1 << sd << s2;
//    g.Direct(lat1,lon1,azi1,sd,lat2,lon2);
//    QPointF q(GeoTools::LatLon2Meters(QPointF(lon2,lat2)));

//    double ax = q.x()-p.x();
//    double ay = q.y()-p.y();
//    double d2a = ax*ax + ay*ay;
//    double bx = r.x()-q.x();
//    double by = r.y()-q.y();
//    double d2b = bx*bx + by*by;
//    double adotb = ax*bx + ay*by;
//    double cos2 = adotb * qAbs(adotb) / (d2a * d2b);

//    if (depth < 1 || cos2 < 0.99995)
//    {
//        points.insert(i+1,q);
//        subdivide(points,i+1,lat1,lon1,azi1,sd,s2,depth+1);
//        subdivide(points,i,lat1,lon1,azi1,s1,sd,depth+1);
//    }
}

void PathEdge::replaceSegments(const QLinkedList<QPointF>& points)
{
    QLinkedList<QPointF>::const_iterator i = points.begin();
    QLinkedList<PathEdgeSegment*>::iterator si = segments.begin();
    while (true)
    {
        if (i == points.end())
            break;
        if ((i+1) == points.end())
            break;
        PathEdgeSegment *s;
        if (si != segments.end())
        {
            s = *si;
            si++;
        }
        else
        {
            s = new PathEdgeSegment(this);
            segments.append(s);
        }
        //qDebug() << GeoTools::Meters2LatLon(*i) << "->" << GeoTools::Meters2LatLon(*(i+1));
        QPointF t1 = path->mapFromScene(*i);
        QPointF t2 = path->mapFromScene(*(i+1));

        double w = GeoTools::projectionWidth();

        // move the points to the world of this path
        qreal m;
        m = fmod(t1.x() + w/2,w);
        if (m < 0)
            t1.setX(w/2+m);
        else
            t1.setX(-w/2+m);

        m = fmod(t2.x() + w/2,w);
        if (m < 0)
            t2.setX(w/2+m);
        else
            t2.setX(-w/2+m);

        if (qAbs(t2.x() - w - t1.x()) < qAbs(t2.x() - t1.x()))
            t2 = t2 - QPointF(w,0);
        else if (qAbs(t2.x() + w - t1.x()) < qAbs(t2.x() - t1.x()))
            t2 = t2 + QPointF(w,0);

        s->setLine(QLineF(t1,t2));
        i++;
    }
    while (si != segments.end())
    {
        delete *si;
        si = segments.erase(si);
    }
}

const QList<PathGraphicsItem*>& PathEdge::pathGraphicsItems() const
{
    return path->graphicItems();
}

void PathEdge::updateSegments(bool fast)
{
    fastUpdate = fast;
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();
    GeoTools gt;
    QPointF q1 = path->mapToScene(p1);
    QPointF q2 = path->mapToScene(p2);
    QPointF r1 = gt.Meters2LatLon(q1);
    QPointF r2 = gt.Meters2LatLon(q2);
    double azi1, azi2, s12;
    g.Inverse(r1.y(),r1.x(),r2.y(),r2.x(),s12,azi1,azi2);

    QLinkedList<QPointF> points;
    points.append(q1);
    points.append(q2);
    subdivide(points,points.begin(),r1.y(),r1.x(),azi1,0,s12,0);

    replaceSegments(points);
    qDebug() << "segments.size:" << segments.size();
}

double PathEdge::length() const
{
    QPointF q1 = path->mapToScene(p1);
    QPointF q2 = path->mapToScene(p2);
    GeoTools gt;
    q1 = gt.Meters2LatLon(p1);
    q2 = gt.Meters2LatLon(p2);
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();
    double azi1, azi2,s12;
    g.Inverse(q1.y(),q1.x(),q2.y(),q2.x(),s12,azi1,azi2);

    return s12;
}

