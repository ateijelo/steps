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
    const QString WindowStatus = "WindowStatus";
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

namespace WindowStates
{
    const QString Minimized = "Minimized";
    const QString Maximized = "Maximized";
    const QString Normal = "Normal";
}

#endif // CONSTANTS_H
