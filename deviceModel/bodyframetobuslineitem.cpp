#include "bodyframetobuslineitem.h"
#include <QDebug>
BodyFrameToBusLineItem::BodyFrameToBusLineItem(const BusGraphicsItem* bus, QGraphicsItem *parent):
    QGraphicsItem(parent),
    bus(bus),
    bodyFrameGraphicsItem(parent)
{

}

QRectF BodyFrameToBusLineItem::boundingRect() const
{
    return QRectF(-1 * penWidth / 2,
                  -1 * (bus->y() - bodyFrameGraphicsItem->y() - bodyFrameGraphicsItem->boundingRect().height())/2,
                  penWidth,
                  (bus->y() - bodyFrameGraphicsItem->y() - bodyFrameGraphicsItem->boundingRect().height()));
}

void BodyFrameToBusLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setWidth(penWidth);
    painter->setPen(pen);
    painter->drawLine(0,
                      -1 * (bus->y() - bodyFrameGraphicsItem->y() - bodyFrameGraphicsItem->boundingRect().height())/2,
                      0,
                      (bus->y() - bodyFrameGraphicsItem->y() - bodyFrameGraphicsItem->boundingRect().height())/2);
    painter->save();
    painter->restore();
}
