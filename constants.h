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
    const QString WindowPosition = "WindowPosition";
    const QString WindowSize = "WindowSize";
    const QString WindowIsMaximized = "WindowIsMaximized";
    const QString ShowLatLonAsToolTip = "ShowLatLonAsToolTip";
}

namespace MapTypes
{
    const QString GoogleMap = "GoogleMap";
    const QString GoogleSat = "GoogleSat";
    const QString GoogleHyb = "GoogleHyb";
}

namespace WheelOptions
{
    const QString ZoomsWithNoKeyPressed = "ZoomsWithNoKeyPressed";
    const QString ZoomsWithCtrlKeyPressed = "ZoomsWithCtrlKeyPressed";
}

#endif // CONSTANTS_H
