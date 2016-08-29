/*
 * constants.h -- part of Steps, a simple maps app
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace SettingsKeys
{
    const QString CachePath = "CachePath";
    const QString MapType = "MapType";
    const QString ZoomLevel = "ZoomLevel";
    const QString Latitude = "Latitude";
    const QString Longitude = "Longitude";
    const QString Angle = "Angle";
    const QString WheelOption = "Wheel";
    const QString WindowGeometry = "WindowGeometry";
    const QString WindowState = "WindowState";
    const QString ShowLatLonAsToolTip = "ShowLatLonAsToolTip";
    const QString RecentPaths = "RecentPaths";
    const QString path = "path";
    const QString MBTilesPath = "MBTilesPath";
    const QString KMLFilePath = "KMLFilePath";
}

namespace WheelOptions
{
    const QString ZoomsWithNoKeyPressed = "ZoomsWithNoKeyPressed";
    const QString ZoomsWithCtrlKeyPressed = "ZoomsWithCtrlKeyPressed";
}

#endif // CONSTANTS_H
