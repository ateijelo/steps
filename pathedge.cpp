#include <QtDebug>
#include <GeographicLib/GeodesicLine.hpp>

#include "pathedge.h"
#include "pathedgesegment.h"
#include "pathgraphicsitem.h"
#include "geotools.h"

PathEdge::PathEdge(const QPointF &p1, const QPointF &p2, PathGraphicsItem *parentPath)
    : QGraphicsItem(parentPath), p1(p1), p2(p2), parentPath(parentPath)
{
    setFlag(ItemHasNoContents);
    updateSegments();
}

//void PathEdge::setParentPath(PathGraphicsItem *path)
//{
//    parentPath = path;
//    foreach (PathEdgeSegment *s, segments)
//        s->setParentPath(path);
//}

QRectF PathEdge::boundingRect() const
{
    return QRectF();
}

void PathEdge::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

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
    return segments.first()->line().angle();
}

qreal PathEdge::angle2() const
{
    return segments.last()->line().angle();
}

void PathEdge::subdivide(QLinkedList<QPointF>& points, QLinkedList<QPointF>::iterator i,
                         double lat1, double lon1, double azi1, double s1, double s2, int depth)
{
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();
    if (depth > 4)
        return;
    if (fastUpdate)
        return;
    QPointF p = *i;
    QPointF r = *(i+1);
    double lat2,lon2;
    //double s12 = s2 - s1;
    double d = 0;
    //double d = (((double)qrand()/RAND_MAX)*0.2 - 0.1)*s12;
    double sd = (s1+s2)/2+d;
    //qDebug() << s1 << sd << s2;
    g.Direct(lat1,lon1,azi1,sd,lat2,lon2);
    QPointF q(GeoTools::LatLon2Meters(QPointF(lon2,lat2)));

//    double ax = q.x()-p.x();
//    double ay = q.y()-p.y();
//    double d2a = ax*ax + ay*ay;
//    double bx = r.x()-q.x();
//    double by = r.y()-q.y();
//    double d2b = bx*bx + by*by;
//    double adotb = ax*bx + ay*by;
//    double cos2 = adotb * qAbs(adotb) / (d2a * d2b);

//    if (cos2 < 0.9995)
    {
        points.insert(i+1,q);
        subdivide(points,i+1,lat1,lon1,azi1,sd,s2,depth+1);
        subdivide(points,i,lat1,lon1,azi1,s1,sd,depth+1);
    }
}

void PathEdge::updateSegments(bool fast)
{
    fastUpdate = fast;
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();
    GeoTools gt;
    QPointF q1 = mapToScene(p1);
    QPointF q2 = mapToScene(p2);
    QPointF r1 = gt.Meters2LatLon(q1);
    QPointF r2 = gt.Meters2LatLon(q2);
    double azi1, azi2, s12;
    g.Inverse(r1.y(),r1.x(),r2.y(),r2.x(),s12,azi1,azi2);

    QLinkedList<QPointF> points;
    points.append(q1);
    points.append(q2);
    subdivide(points,points.begin(),r1.y(),r1.x(),azi1,0,s12,0);

    QLinkedList<PathEdgeSegment*>::iterator si = segments.begin();
    QLinkedList<QPointF>::iterator i = points.begin();
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
            s->setParentEdge(this);
            s->setParentPath(parentPath);
            segments.append(s);
        }
        //qDebug() << GeoTools::Meters2LatLon(*i) << "->" << GeoTools::Meters2LatLon(*(i+1));
        QPointF t1 = mapFromScene(*i);
        QPointF t2 = mapFromScene(*(i+1));

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
    qDebug() << "segments.size:" << segments.size();
}

double PathEdge::length() const
{
    QPointF q1 = mapToScene(p1);
    QPointF q2 = mapToScene(p2);
    GeoTools gt;
    q1 = gt.Meters2LatLon(p1);
    q2 = gt.Meters2LatLon(p2);
    const GeographicLib::Geodesic& g = GeographicLib::Geodesic::WGS84();
    double azi1, azi2,s12;
    g.Inverse(q1.y(),q1.x(),q2.y(),q2.x(),s12,azi1,azi2);

    return s12;
}

