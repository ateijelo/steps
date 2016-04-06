#include "geotools.h"
#include "path.h"


Path::Path(const QPointF &from, const QPointF &to, QObject *parent) :
    QObject(parent)
{
    center = new PathGraphicsItem(from, to);
    left = new PathGraphicsItem(from, to);
    right = new PathGraphicsItem(from, to);

    center->setPath(this);
    center->addSibling(left);
    center->addSibling(right);

    _length = center->length();
    _name = "";
    _visible = true;
}

void Path::addToScene(QGraphicsScene *scene)
{
    auto w = GeoTools::projectionWidth();

    center->setZValue(2);
    center->setPos(QPointF(0,0));
    right->setZValue(2);
    right->setPos(QPointF(w,0));
    left->setZValue(2);
    left->setPos(QPointF(-w,0));

    scene->addItem(center);
    scene->addItem(right);
    scene->addItem(left);
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
