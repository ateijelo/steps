#ifndef GEOCIRCLE_H
#define GEOCIRCLE_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>

class GeoCircle;

class GeoCircleNode : public QGraphicsEllipseItem
{
    public:
        GeoCircleNode(QGraphicsItem *parent = 0);
        void setParentCircle(GeoCircle *circle);
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    private:
        GeoCircle *parentCircle;
};

class GeoCircleLine : public QGraphicsLineItem
{
    public:
        GeoCircleLine(QGraphicsItem *parent = 0);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class GeoCircle : public QGraphicsItem
{
    public:
        GeoCircle(QGraphicsItem *parent = 0);
        void nodeMoved(GeoCircleNode *node);
        QRectF boundingRect() const;
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    private:
        void updateSegments();
        void addCardinalPoint(const QString& label);

        GeoCircleNode *center;
        GeoCircleNode *target;
        QList<GeoCircleLine*> segments;
        QList<QGraphicsTextItem*> cardinalLabels;
        QList<QGraphicsEllipseItem*> cardinalDots;
};

#endif // GEOCIRCLE_H
