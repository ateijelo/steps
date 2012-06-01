// Utility functions for Qt GMaps
// Copyright (C) 2009 Andy Teijelo <andy@teijelo.net>
//
// This class defines several functions to work with the WGS84
// ellipsoid, with the Mercator projection used by GoogleMap,
// with its tiling system, etc.
// Code was taken from:
//  http://www.maptiler.org/google-maps-coordinates-tile-bounds-projection/globalmaptiles.py
//
// and converted from Python.
// The Python program above includes the following notice:
//  Copyright (c) 2008 Klokan Petr Pridal. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
// For more information, visit:
//  http://www.klokan.cz/projects/gdal2tiles/

// This class also includes an adaptation from the JavaScript implementation
// of the Vincenty formula for the distance between two lat/lon points
// found in:
//  http://www.movable-type.co.uk/scripts/latlong-vincenty.html
//
// The code is offered there under the LGPL license
// (http://creativecommons.org/licenses/LGPL/2.1/)
// and includes the following notice:
//  Copyright © 2002-2009 Chris Veness

#include <math.h>

#include "geotools.h"

#define originShift (M_PI * 6378137)
#define TILESIZE (256)

double GeoTools::_projWidth = 0;
double GeoTools::initialResolution = 2 * M_PI * 6378137 / TILESIZE;

GeoTools::GeoTools(int tileSize)
        : tileSize(tileSize)
{
    // original code said: self.originShift = 2 * math.pi * 6378137 / 2.0
}

// This function converts lat/lon to meter coordinates
// in the Spherical Mercator EPSG:900913
// It's modified to make meter coordinates grow right and down
// like screen coordinates.
// l.x() is longitude
// l.y() is latitude
QPointF GeoTools::LatLon2Meters(const QPointF& l)
{
    double mx,my;
    mx = l.x() * originShift / 180.0;
    my = log(tan((90.0 + l.y()) * M_PI / 360.0)) / (M_PI / 180.0);
    my = - my * originShift / 180.0;
    return QPointF(mx,my);
}

// Converts XY point from Spherical Mercator EPSG:900913 to lat/lon in WGS84 Datum
QPointF GeoTools::Meters2LatLon(const QPointF& m)
{
    double lat,lon;
    lon =  (m.x() / originShift) * 180.0;
    lat = -(m.y() / originShift) * 180.0;
    lat = 180 / M_PI * (2 * atan( exp( lat * M_PI / 180.0)) - M_PI / 2.0);
    return QPointF(lon,lat);
}

double GeoTools::projectionWidth()
{
    if (_projWidth == 0)
        _projWidth = Pixels2Meters(QPointF(256,256),0).x() - Pixels2Meters(QPointF(0,0),0).x();
    return _projWidth;
}

// Gives how many projection meters 1 pixel represents at given zoom leven
double GeoTools::resolution(int zoom)
{
    return initialResolution / (1 << zoom);
}

// Converts EPSG:900913 to pyramid pixel coordinates in given zoom level
QPointF GeoTools::Meters2Pixels(const QPointF& m, int zoom) const
{
    double res = resolution(zoom);
    return QPointF((m.x() + originShift) / res,(m.y() + originShift) / res);
}

// Converts pixel coordinates to projection meters
QPointF GeoTools::Pixels2Meters(const QPointF& p, int zoom)
{
    double res = resolution(zoom);
    return QPointF(p.x()*res - originShift, p.y()*res - originShift);
}

// Returns the Google Tile coordinates that contain the given pixel
QPoint GeoTools::Pixels2GoogleTile(const QPointF &p) const
{
    return QPoint(int(ceil(p.x()/tileSize) - 1),int(ceil(p.y()/tileSize) - 1));
}

// Returns the Google Tile that contains the given
// projection point at the given zoom level
QPoint GeoTools::Meters2GoogleTile(const QPointF& m, int zoom) const
{
    return Pixels2GoogleTile(Meters2Pixels(m,zoom));
}

QPointF GeoTools::GoogleTile2Pixels(int x, int y) const
{
    return QPointF(x*tileSize,y*tileSize);
}

QPointF GeoTools::GoogleTile2Pixels(const QPoint &g) const
{
    return GoogleTile2Pixels(g.x(),g.y());
}

QPointF GeoTools::GoogleTile2Meters(int x, int y, int zoom) const
{
    return Pixels2Meters(GoogleTile2Pixels(x,y),zoom);
}

QPointF GeoTools::GoogleTile2Meters(const QPoint &g, int zoom) const
{
    return GoogleTile2Meters(g.x(),g.y(),zoom);
}

double GeoTools::VincentyDistance(const QPointF& from, const QPointF& to) const
{
    double L = (to.x() - from.x()) * DEG_TO_RAD;
    double U1 = atan((1-FLATTENING) * tan(from.y() * DEG_TO_RAD));
    double U2 = atan((1-FLATTENING) * tan(to.y() * DEG_TO_RAD));
    double sinU1 = sin(U1);
    double cosU1 = cos(U1);
    double sinU2 = sin(U2);
    double cosU2 = cos(U2);

    double lambda = L;
    double lambdaP, iterLimit = 100;
    double sinSigma = 0;
    double cosSigma = 0;
    double sigma = 0;
    double cosSqAlpha = 0;
    double cos2SigmaM = 0;
    do
    {
        double sinLambda = sin(lambda);
        double cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) +
                               (cosU1*sinU2 - sinU1*cosU2*cosLambda) * (cosU1*sinU2 - sinU1*cosU2*cosLambda));
        if (sinSigma == 0)
            return 0; // co-incident points
        cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1 - sinAlpha*sinAlpha;
        if (cosSqAlpha == 0)
            cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (§6)
        else
            cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;
        double C = FLATTENING/16*cosSqAlpha*(4+FLATTENING*(4-3*cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1-C) * FLATTENING * sinAlpha * (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
    }
    while (fabs(lambda-lambdaP) > 1e-12 && --iterLimit>0);

    if (iterLimit==0) return -1;  // formula failed to converge

    double uSq = cosSqAlpha *
                 (MAJOR_EARTH_RADIUS_IN_METERS*MAJOR_EARTH_RADIUS_IN_METERS - MINOR_EARTH_RADIUS_IN_METERS*MINOR_EARTH_RADIUS_IN_METERS) / (MINOR_EARTH_RADIUS_IN_METERS*MINOR_EARTH_RADIUS_IN_METERS);
    double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
    double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
    double deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
            B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
    double s = MINOR_EARTH_RADIUS_IN_METERS*A*(sigma-deltaSigma);

    //s = s.toFixed(3); // round to 1mm precision
    return s;
}

double GeoTools::HaversineDistance(const QPointF& from, const QPointF& to) const
{
    qreal latitudeArc = (from.y() - to.y()) * DEG_TO_RAD;
    qreal longitudeArc = (from.x() - to.x()) * DEG_TO_RAD;
    double latitudeH = sin(latitudeArc * 0.5);
    latitudeH *= latitudeH;
    double lontitudeH = sin(longitudeArc * 0.5);
    lontitudeH *= lontitudeH;
    double tmp = cos(from.y()*DEG_TO_RAD) * cos(to.y()*DEG_TO_RAD);
    return 2.0 * asin(sqrt(latitudeH + tmp*lontitudeH)) * EARTH_RADIUS_IN_METERS;
}

double GeoTools::Distance(const QPointF& from, const QPointF& to) const
{
    return HaversineDistance(from, to);
}
