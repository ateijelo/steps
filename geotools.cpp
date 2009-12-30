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

GeoTools::GeoTools(int tileSize)
        : tileSize(tileSize)
{
    initialResolution = 2 * M_PI * 6378137 / tileSize;
    // original code said: self.originShift = 2 * math.pi * 6378137 / 2.0
    originShift = M_PI * 6378137;
}

// This function converts lat/lon to meter coordinates
// in the Spherical Mercator EPSG:900913
void GeoTools::LatLon2Meters(double lat, double lon, double *_mx, double *_my)
{
    double mx,my;
    mx = lon * originShift / 180.0;
    my = log(tan((90.0 + lat) * M_PI / 360.0)) / (M_PI / 180.0);
    my = my * originShift / 180.0;
    (*_mx) = mx;
    (*_my) = my;
}

double GeoTools::resolution(int zoom)
{
    return initialResolution / (1 << zoom);
}

// Converts EPSG:900913 to pyramid pixel coordinates in given zoom level
void GeoTools::Meters2Pixels(double mx, double my, int zoom, double *_px, double *_py)
{
    double px,py;
    double res = resolution(zoom);
    px = (mx + originShift) / res;
    py = (my + originShift) / res;
    (*_px) = px;
    (*_py) = py;
}

// Returns the TMS tile that contains the given pixel coordinates
void GeoTools::Pixels2TMSTile(double px, double py, int *_tx, int *_ty)
{
    (*_tx) = int(ceil(px/tileSize) - 1);
    (*_ty) = int(ceil(py/tileSize) - 1);
}

// Returns the Google Tile coordinates that contain the given pixel
void GeoTools::Pixels2GoogleTile(double px, double py, int zoom, int *_gx, int *_gy)
{
    (*_gx) = int(ceil(px/tileSize) - 1);
    (*_gy) = (1 << zoom) - 1 - int(ceil(py/tileSize) - 1);
}

// Returns TMS tile for given mercator coordinates
void GeoTools::Meters2TMSTile(double mx, double my, int zoom, int *_tx, int *_ty)
{
    double px,py;
    Meters2Pixels(mx,my,zoom,&px,&py);
    Pixels2TMSTile(px,py,_tx,_ty);
}

// Converts TMS tile coordinates to Google Tile coordinates
void GeoTools::TMS2Google(int tx, int ty, int zoom, int *_gx, int *_gy)
{
    (*_gx) = tx;
    (*_gy) = (1 << zoom) - 1 - ty;
}
