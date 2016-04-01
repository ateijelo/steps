#ifndef PATH_H
#define PATH_H

#include <QObject>
#include <QGraphicsScene>

#include "pathgraphicsitem.h"

class Path : public QObject
{
    Q_OBJECT
public:
    explicit Path(const QPointF& from,
                  const QPointF& to,
                  QObject *parent = 0);

    void addToScene(QGraphicsScene *scene);

    QString name() const { return _name; }
    void setName(const QString& name);

    bool isVisible() const { return _visible; }
    void setVisible(bool visible);

    double length() const { return _length; }
    void setLength(double l);

signals:
    void nameChanged(const QString&);
    void visibleChanged(bool);
    void lengthChanged(double length);

public slots:
    

private:
    PathGraphicsItem *left, *center, *right;
    QString _name;
    bool _visible;
    double _length;
};

#endif // PATH_H
