#include "path.h"

Path::Path(const QPointF &from, const QPointF &to, QObject *parent) :
    QObject(parent)
{
    center = new PathGraphicsItem(from, to);
    center->setPath(this);

    _length = center->length();
    _name = "";
    _visible = true;
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

void Path::setLength(double l)
{
    if (_length != l) {
        _length = l;
        emit lengthChanged(l);
    }
}
