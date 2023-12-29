#include "lrmgrphicsitem.h"
#include "monitor/monitorWidget.h"
#include <QPainter>
#include <QRectF>
#include <QMenu>

#include <QLineF>
#include <QtMath>
#include <QMenu>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include "deviceModel/innerbodyframescene.h"


LRMGraphicsItem::LRMGraphicsItem(const BusGraphicsItem *Ax, const BusGraphicsItem *Ay, const BusGraphicsItem *Bx, const BusGraphicsItem *By, const Module& module):
    module(module),
    toAx(Ax, this),
    toAy(Ay, this),
    toBx(Bx, this),
    toBy(By, this)
{
    this->setZValue(1);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    rect_bound = boundingRect();
    rect_cpu = rect_bound.adjusted(15,10,-15,-90);
    rect_659 = rect_bound.adjusted(8,70,-8,-8);
    QPointF startPoint1(rect_cpu.bottomLeft().x()+10,rect_cpu.bottom());
    QPointF endPoint1(startPoint1.x(),rect_659.top());
    QPointF startPoint2(rect_659.topRight().x()-17,rect_659.top());
    QPointF endPoint2(startPoint2.x(),rect_cpu.bottom());
    arrowPath_first = createArrow(startPoint1,endPoint1);
    arrowPath_second = createArrow(startPoint2,endPoint2);
    //InnerBodyFrameScene* scene = dynamic_cast<InnerBodyFrameScene*>(this->scene());
    //不好的设计，不要在构造函数中将this指针传递出去
    //monitorWidget = new MonitorWidget(*this, dataFrames, );
}

LRMGraphicsItem::~LRMGraphicsItem()
{
    delete monitorWidget;
}

void LRMGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     QString text_659 = QStringLiteral("ARINC659协议信号处理模块");
     QString text_cpu = QStringLiteral("CPU");

     QTextOption text_style(Qt::AlignCenter);
     text_style.setWrapMode(QTextOption::WordWrap);//设置文本格式：自动换行且居中

     painter->setRenderHint(QPainter::Antialiasing);//绘图抗锯齿

     painter->setPen(QPen(Qt::black));
     painter->drawRect(this->getRect_bound());
     painter->setBrush(QBrush(Qt::gray));
     painter->drawRect(this->getRect_cpu());
     painter->drawRect(this->getRect_659());
     painter->drawText(this->getRect_659(),text_659,text_style);
     painter->drawText(this->rect_cpu,text_cpu,text_style);
//     QPointF startPoint1(rect_cpu.bottomLeft().x()+10,rect_cpu.bottom());
//     QPointF endPoint1(startPoint1.x(),rect_659.top());
//     QPointF startPoint2(rect_659.topRight().x()-17,rect_659.top());
//     QPointF endPoint2(startPoint2.x(),rect_cpu.bottom());


     QPen* myPen = new QPen();
     myPen->setColor(Qt::black);
     myPen->setCapStyle(Qt::RoundCap);
     myPen->setJoinStyle(Qt::RoundJoin);
     painter->setPen(*myPen);//箭头画笔设置


     QLinearGradient* arrowGradient = new QLinearGradient(startPoint1,endPoint1);
     arrowGradient->setColorAt(0,QColor("#f093fb"));
     arrowGradient->setColorAt(1,QColor("#f5576c"));
     painter->setBrush(QBrush(*arrowGradient));//箭头渐变式画刷设置



     painter->drawPath(arrowPath_first);
     painter->drawPath(arrowPath_second);

     computeLineToBus();

//     QLineF mainLine(startPoint1,endPoint1);
//     //painter->drawLine(mainLine);
//     QLineF leftArrow;
//     leftArrow.setP1(startPoint1);
//     leftArrow.setLength(mainLine.length()/5.0);
//     leftArrow.setAngle(mainLine.angle()-36.87);
//     QLineF leftBackArrow;
//     leftBackArrow.setP1(leftArrow.p2());
//     leftBackArrow.setLength(leftArrow.length()/3.0);
//     leftBackArrow.setAngle(mainLine.normalVector().angle());
//     QLineF leftVertical;
//     leftVertical.setP1(leftBackArrow.p2());
//     leftVertical.setLength(mainLine.length()-leftArrow.length()*cos(qDegreesToRadians(36.87)));
//     leftVertical.setAngle(mainLine.angle());
//     QLineF rightArrow;
//     rightArrow.setP1(startPoint1);
//     rightArrow.setLength(mainLine.length()/5.0);
//     rightArrow.setAngle(mainLine.angle()+36.87);
//     QLineF rightBackArrow;
//     rightBackArrow.setP1(rightArrow.p2());
//     rightBackArrow.setLength(rightArrow.length()/3.0);
//     rightBackArrow.setAngle(mainLine.normalVector().angle()+180.0);
//     QLineF rightVertical;
//     rightVertical.setP1(rightBackArrow.p2());
//     rightVertical.setLength(mainLine.length()-rightArrow.length()*cos(qDegreesToRadians(36.87)));
//     rightVertical.setAngle(mainLine.angle());
//     QLineF bottomLine(leftVertical.p2(),rightVertical.p2());

//     QPainterPath arrowPath(startPoint1);
//     arrowPath.lineTo(leftArrow.p2());
//     arrowPath.lineTo(leftBackArrow.p2());
//     arrowPath.lineTo(leftVertical.p2());
//     arrowPath.lineTo(bottomLine.p2());
//     arrowPath.lineTo(rightVertical.p1());
//     arrowPath.lineTo(rightBackArrow.p1());
//     arrowPath.lineTo(rightArrow.p1());

//     painter->drawPath(arrowPath_first);
//     painter->drawPath(arrowPath_second);

//     painter->drawLine(leftArrow);
//     painter->drawLine(leftBackArrow);
//     painter->drawLine(leftVertical);
//     painter->drawLine(rightArrow);
//     painter->drawLine(rightBackArrow);
//     painter->drawLine(rightVertical);
//     painter->drawLine(bottomLine);




//     const QPointF points1[7]= {
//         startPoint1,
//         QPointF(startPoint1.x()-3,startPoint1.y()+4),
//         QPointF(startPoint1.x()-1.5,startPoint1.y()+4),
//         QPointF(endPoint1.x()-1.5,endPoint1.y()),
//         QPointF(endPoint1.x()+1.5,endPoint1.y()),
//         QPointF(startPoint1.x()+1.5,startPoint1.y()+4),
//         QPointF(startPoint1.x()+3,startPoint1.y()+4),
//     };

//     const QPointF points2[7]= {
//         startPoint2,
//         QPointF(startPoint2.x()-3,startPoint2.y()-4),
//         QPointF(startPoint2.x()-1.5,startPoint2.y()-4),
//         QPointF(endPoint2.x()-1.5,endPoint2.y()),
//         QPointF(endPoint2.x()+1.5,endPoint2.y()),
//         QPointF(startPoint2.x()+1.5,startPoint2.y()-4),
//         QPointF(startPoint2.x()+3,startPoint2.y()-4),
//     };
     

//     painter->setBrush(QBrush(Qt::red));
//     painter->drawPolygon(points1,7);
//     painter->drawPolygon(points2,7);
}

void LRMGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *cfgModule = new QAction("配置模块");
    QAction *deleteModule = new QAction("删除模块");
    QAction *watchWave = new QAction("查看波形");
    connect(cfgModule, &QAction::triggered, this, [=](){
       emit cfgModuleSignal(module.getModuleNumber());
    });
    connect(deleteModule, &QAction::triggered, this, [=](){
       emit deleteModuleSignal(module.getModuleNumber());
    });
    connect(watchWave, &QAction::triggered, this, [=](){
        monitorWidget->setWindowFlags(Qt::Dialog);
        monitorWidget->show();
    });
    menu.addAction(cfgModule);
    menu.addAction(deleteModule);
    menu.addAction(watchWave);
    menu.exec(QCursor::pos());
    QGraphicsItem::contextMenuEvent(event);
}

const BodyFrameToBusLineItem &LRMGraphicsItem::getToAx() const
{
    return toAx;
}

const BodyFrameToBusLineItem &LRMGraphicsItem::getToAy() const
{
    return toAy;
}

const BodyFrameToBusLineItem &LRMGraphicsItem::getToBx() const
{
    return toBx;
}

const BodyFrameToBusLineItem &LRMGraphicsItem::getToBy() const
{
    return toBy;
}

MonitorWidget *LRMGraphicsItem::getMonitorWidget() const
{
    return monitorWidget;
}

const Module &LRMGraphicsItem::getModule() const
{
    return module;
}

void LRMGraphicsItem::initMonitorWidget()
{
    InnerBodyFrameScene* scene = dynamic_cast<InnerBodyFrameScene*>(this->scene());
    monitorWidget = new MonitorWidget(*this, scene->getDataFrames(), scene->getModuleGraphicItems());
}

QPainterPath LRMGraphicsItem::createArrow(QPointF startPoint, QPointF endPoint)
{
    QLineF mainLine(startPoint,endPoint);
    //painter->drawLine(mainLine);
    QLineF leftArrow;
    leftArrow.setP1(startPoint);
    leftArrow.setLength(mainLine.length()/5.0);
    leftArrow.setAngle(mainLine.angle()-36.87);
    QLineF leftBackArrow;
    leftBackArrow.setP1(leftArrow.p2());
    leftBackArrow.setLength(leftArrow.length()/3.0);
    leftBackArrow.setAngle(mainLine.normalVector().angle());
    QLineF leftVertical;
    leftVertical.setP1(leftBackArrow.p2());
    leftVertical.setLength(mainLine.length()-leftArrow.length()*cos(qDegreesToRadians(36.87)));
    leftVertical.setAngle(mainLine.angle());
    QLineF rightArrow;
    rightArrow.setP1(startPoint);
    rightArrow.setLength(mainLine.length()/5.0);
    rightArrow.setAngle(mainLine.angle()+36.87);
    QLineF rightBackArrow;
    rightBackArrow.setP1(rightArrow.p2());
    rightBackArrow.setLength(rightArrow.length()/3.0);
    rightBackArrow.setAngle(mainLine.normalVector().angle()+180.0);
    QLineF rightVertical;
    rightVertical.setP1(rightBackArrow.p2());
    rightVertical.setLength(mainLine.length()-rightArrow.length()*cos(qDegreesToRadians(36.87)));
    rightVertical.setAngle(mainLine.angle());
    QLineF bottomLine(leftVertical.p2(),rightVertical.p2());

    QPainterPath arrowPath(startPoint);
    arrowPath.lineTo(leftArrow.p2());
    arrowPath.lineTo(leftBackArrow.p2());
    arrowPath.lineTo(leftVertical.p2());
    arrowPath.lineTo(bottomLine.p2());
    arrowPath.lineTo(rightVertical.p1());
    arrowPath.lineTo(rightBackArrow.p1());
    arrowPath.lineTo(rightArrow.p1());
    return arrowPath;
}

void LRMGraphicsItem::computeLineToBus()
{
    qreal width = this->boundingRect().width() ;
    qreal height = this->boundingRect().height();
    toAx.setPos(width / 5, (2470 - this->y() + height) / 2);
    toAy.setPos(width * 2 / 5, (2490 - this->y() + height) / 2);
    toBx.setPos(width * 3 / 5, (2510 - this->y() + height) / 2);
    toBy.setPos(width * 4 / 5, (2530 - this->y() + height) / 2);
//    qDebug() << this->x() << toAx.x() << toAy.x() << toBx.x() << toBy.x();
    toAx.update();
    toAy.update();
    toBx.update();
    toBy.update();
}

//QVariant LRMGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//    if(this->scene() && change == QGraphicsItem::ItemPositionChange){
//        QPointF newPos = value.toPointF();
//        QRectF limitRec(500, 0, 4000 - this->boundingRect().height(), 2470 - this->boundingRect().width());
//        if(limitRec.contains(newPos) == false){
//            newPos.setX()
//        }
//    }


//    return  QGraphicsItem::itemChange(change,value);
//}



QRectF LRMGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, 80, 120);
}


QRectF LRMGraphicsItem::getRect_bound() const
{
    return rect_bound;
}

void LRMGraphicsItem::setRect_bound(const QRectF &value)
{
    rect_bound = value;
}

QRectF LRMGraphicsItem::getRect_cpu() const
{
    return rect_cpu;
}

void LRMGraphicsItem::setRect_cpu(const QRectF &value)
{
    rect_cpu = value;
}

QRectF LRMGraphicsItem::getRect_659() const
{
    return rect_659;
}

void LRMGraphicsItem::setRect_659(const QRectF &value)
{
    rect_659 = value;
}
