#include "path.h"

Path::Path(QObject *parent) :
    QObject(parent)
{
    center = new PathGraphicsItem();
    center->setPath(this);
}

void Path::setName(const QString &name)
{
    if (_name != name) {
        _name = name;
        emit nameChanged(name);
    }
}

void Path::setVisible(bool visible)
{
    if (_visible != visible) {
        _visible = visible;
        emit visibleChanged(visible);
    }
}
