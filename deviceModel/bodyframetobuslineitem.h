#ifndef BODYFRAMETOBUSLINEITEM_H
#define BODYFRAMETOBUSLINEITEM_H

#include<qgraphicsitem.h>
#include<QPainter>
#include"busgraphicsitem.h"



class BodyFrameToBusLineItem : public QObject, public QGraphicsItem
{
public:
    BodyFrameToBusLineItem(const BusGraphicsItem* bus, QGraphicsItem* parent);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    const BusGraphicsItem *getBus() const;

private:
    qreal penWidth = 5;
    const BusGraphicsItem* bus = nullptr;
    QGraphicsItem* bodyFrameGraphicsItem = nullptr;
};

#endif // BODYFRAMETOBUSLINEITEM_H
