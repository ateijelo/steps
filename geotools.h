#ifndef GEOTOOLS_H
#define GEOTOOLS_H

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

#include <QPoint>
#include <QPointF>

/// The usual PI/180 constant
#define DEG_TO_RAD 0.017453292519943295769236907684886
/// Earth's quatratic mean radius for WGS-84
#define EARTH_RADIUS_IN_METERS 6372797.560856

/// Major semiaxis of the ellipsoid
#define MAJOR_EARTH_RADIUS_IN_METERS 6378137.0
/// Minor semiaxis of the ellipsoid
#define MINOR_EARTH_RADIUS_IN_METERS 6356752.3142
/// Flattening
#define FLATTENING 1/298.257223563

class GeoTools
{
    public:
        GeoTools(int tileSize=256);

        double resolution(int zoom) const;
        static QPointF LatLon2Meters(const QPointF& l);
        static QPointF Meters2LatLon(const QPointF& m);
        QPointF Meters2Pixels(const QPointF& m, int zoom) const;
        QPointF Pixels2Meters(const QPointF& p, int zoom) const;
        QPoint Pixels2GoogleTile(const QPointF& p) const;
        QPointF GoogleTile2Pixels(int x, int y) const;
        QPointF GoogleTile2Pixels(const QPoint& g) const;
        QPoint Meters2GoogleTile(const QPointF& m, int zoom) const;
        QPointF GoogleTile2Meters(int x, int y, int zoom) const;
        QPointF GoogleTile2Meters(const QPoint& g, int zoom) const;
        double VincentyDistance(const QPointF& from, const QPointF& to) const;
        double HaversineDistance(const QPointF& from, const QPointF& to) const;
        double Distance(const QPointF& from, const QPointF& to) const;

    private:
        int tileSize;
        double initialResolution;
};

#endif // GEOTOOLS_H
