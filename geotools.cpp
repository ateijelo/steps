/*
 * geotools.cpp -- part of Steps, a simple maps app
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

#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <math.h>

#include "geotools.h"

#define originShift (M_PI * 6378137)
#define TILESIZE (256)

double GeoTools::_projWidth = 0;
double GeoTools::initialResolution = 2 * M_PI * 6378137 / TILESIZE;

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
QPointF GeoTools::Meters2Pixels(const QPointF& m, int zoom)
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
QPoint GeoTools::Pixels2GoogleTile(const QPointF &p)
{
    qDebug() << "Pixels2GoogleTile";
    qDebug() << "    p:" << p;
    QPoint q(int(ceil(p.x()/TILESIZE) - 1),int(ceil(p.y()/TILESIZE) - 1));
    qDebug() << "    q:" << q;
    return q;
}

// Returns the Google Tile that contains the given
// projection point at the given zoom level
QPoint GeoTools::Meters2GoogleTile(const QPointF& m, int zoom)
{
    return Pixels2GoogleTile(Meters2Pixels(m,zoom));
}

QPointF GeoTools::GoogleTile2Pixels(int x, int y)
{
    return QPointF(x*TILESIZE,y*TILESIZE);
}

QPointF GeoTools::GoogleTile2Pixels(const QPoint &g)
{
    return GoogleTile2Pixels(g.x(),g.y());
}

QPointF GeoTools::GoogleTile2Meters(int x, int y, int zoom)
{
    return Pixels2Meters(GoogleTile2Pixels(x,y),zoom);
}

QPointF GeoTools::GoogleTile2Meters(const QPoint &g, int zoom)
{
    return GoogleTile2Meters(g.x(),g.y(),zoom);
}
