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
    //内部填充样式模块
    QRectF testRectf = boundingRect();
    QRectF boundaryRectf = testRectf.adjusted(0,-2,0,3);
    QPoint startPoint(testRectf.left(),testRectf.center().y());//用来规定渐变样式的生效方向
    QPoint endPoint(testRectf.right(),testRectf.center().y());//用来规定渐变样式的生效方向
    QLinearGradient innerGradient(startPoint,endPoint);
    innerGradient.setColorAt(0,QColor("#b35754"));
    innerGradient.setColorAt(0.4,QColor("#FFFFFF"));
    innerGradient.setColorAt(0.6,QColor("#FFFFFF"));
    innerGradient.setColorAt(1,QColor("#b35754"));
    QBrush innerBrush(innerGradient);
    QPen brushPen;
    brushPen.setBrush(innerBrush);
    brushPen.setWidth(penWidth);
    painter->setPen(brushPen);
    painter->drawLine(0,
                      -1 * (bus->y() - bodyFrameGraphicsItem->y() - bodyFrameGraphicsItem->boundingRect().height())/2,
                      0,
                      (bus->y() - bodyFrameGraphicsItem->y() - bodyFrameGraphicsItem->boundingRect().height())/2);


    //外部轮廓样式
    QPen boundaryPen;
    boundaryPen.setColor(QColor("#427c7e"));
    painter->setPen(boundaryPen);
    painter->drawRect(boundaryRectf);


    painter->save();
    painter->restore();
}
