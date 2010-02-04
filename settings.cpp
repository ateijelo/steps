#include <QSettings>

#include "settings.h"

Settings::Settings()
{
    load();
}

QString Settings::getCachePath() const
{
    return cachePath;
}

void Settings::setCachePath(const QString& path)
{
    cachePath = path;
    save();
}

void Settings::load()
{
    QSettings s("ateijelo","QtGMaps");
    cachePath = s.value("CachePath").toString();
    if (cachePath.isNull())
        cachePath = "";
}

void Settings::save()
{
    QSettings s("ateijelo","QtGMaps");
    s.setValue("CachePath",cachePath);
}
