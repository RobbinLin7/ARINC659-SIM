#include "lrmgrphicsitem.h"
#include <QPainter>
#include <QRectF>


LRMGrphicsItem::LRMGrphicsItem()
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    rect_bound = boundingRect();
    rect_cpu = rect_bound.adjusted(15,10,-15,-90);
    rect_659 = rect_bound.adjusted(8,70,-8,-8);
}

void LRMGrphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    //     painter->setBrush(QBrush(Qt::blue));
     QString text_659 = QStringLiteral("ARINC659协议信号处理模块");
     QString text_cpu = QStringLiteral("CPU");

     QTextOption text_style(Qt::AlignCenter);
     text_style.setWrapMode(QTextOption::WordWrap);//设置文本格式：自动换行且居中

     painter->setPen(QPen(Qt::black));
     painter->drawRect(this->getRect_bound());
     painter->setBrush(QBrush(Qt::gray));
     painter->drawRect(this->getRect_cpu());
     painter->drawRect(this->getRect_659());
     painter->drawText(this->getRect_659(),text_659,text_style);
     painter->drawText(this->rect_cpu,text_cpu,text_style);
     QPointF startPoint1(rect_cpu.bottomLeft().x()+10,rect_cpu.bottom());
     QPointF endPoint1(startPoint1.x(),rect_659.top());
     QPointF startPoint2(rect_659.topRight().x()-17,rect_659.top());
     QPointF endPoint2(startPoint2.x(),rect_cpu.bottom());


     const QPointF points1[7]= {
         startPoint1,
         QPointF(startPoint1.x()-3,startPoint1.y()+4),
         QPointF(startPoint1.x()-1.5,startPoint1.y()+4),
         QPointF(endPoint1.x()-1.5,endPoint1.y()),
         QPointF(endPoint1.x()+1.5,endPoint1.y()),
         QPointF(startPoint1.x()+1.5,startPoint1.y()+4),
         QPointF(startPoint1.x()+3,startPoint1.y()+4),
     };

     const QPointF points2[7]= {
         startPoint2,
         QPointF(startPoint2.x()-3,startPoint2.y()-4),
         QPointF(startPoint2.x()-1.5,startPoint2.y()-4),
         QPointF(endPoint2.x()-1.5,endPoint2.y()),
         QPointF(endPoint2.x()+1.5,endPoint2.y()),
         QPointF(startPoint2.x()+1.5,startPoint2.y()-4),
         QPointF(startPoint2.x()+3,startPoint2.y()-4),
     };
     
     painter->setBrush(QBrush(Qt::red));
     painter->drawPolygon(points1,7);
     painter->drawPolygon(points2,7)
;
}

QRectF LRMGrphicsItem::boundingRect() const
{
    return QRectF(0, 0, 80, 120);
}


QRectF LRMGrphicsItem::getRect_bound() const
{
    return rect_bound;
}

void LRMGrphicsItem::setRect_bound(const QRectF &value)
{
    rect_bound = value;
}

QRectF LRMGrphicsItem::getRect_cpu() const
{
    return rect_cpu;
}

void LRMGrphicsItem::setRect_cpu(const QRectF &value)
{
    rect_cpu = value;
}

QRectF LRMGrphicsItem::getRect_659() const
{
    return rect_659;
}

void LRMGrphicsItem::setRect_659(const QRectF &value)
{
    rect_659 = value;
}
