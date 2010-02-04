#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
    public:
        Settings();
        QString getCachePath() const;
        void setCachePath(const QString&);

    public slots:
        void load();
        void save();

    private:
        QString cachePath;
};

extern Settings *settings;

#endif // SETTINGS_H
