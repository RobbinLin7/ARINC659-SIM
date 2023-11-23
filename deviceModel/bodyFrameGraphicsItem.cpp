#include "bodyFrameGraphicsItem.h"

#include <QImageReader>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QToolTip>
#include <QGraphicsScene>

BodyFrameGraphicsItem::~BodyFrameGraphicsItem()
{
    delete treeWidgetItem;
}

BodyFrameGraphicsItem::BodyFrameGraphicsItem(const BusGraphicsItem *Ax, const BusGraphicsItem *Ay, const BusGraphicsItem *Bx, const BusGraphicsItem *By, BodyFrame bodyFrameItem, QGraphicsItem *parent):
    QGraphicsItem(parent),
    img(":/resources/Image/bodyFrame.png"),
    toAx(Ax, this),
    toAy(Ay, this),
    toBx(Bx, this),
    toBy(By, this)
{
    this->bodyFrameItem = bodyFrameItem;
    setZValue(1);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    update();
//    QGraphicsItemGroup* group = new QGraphicsItemGroup();
//    group->addToGroup(this);
//    collidingGroup = std::shared_ptr<QGraphicsItemGroup>(group);
}


QRectF BodyFrameGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, img.width() / 4, img.height() / 4);
}

QPainterPath BodyFrameGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void BodyFrameGraphicsItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *,
                          QWidget *)
{
    painter->drawImage(QRectF(0, 0, img.width() / 4, img.height() / 4), img);
    painter->save();
    painter->restore();
    computeLineToBus();
    //qDebug() << bodyFrameItem.getBodyFrameItemID() << pos();
}

void BodyFrameGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    qDebug() << "contextMenuEvent";
    QMenu menu;
    QAction *cfgBF = new QAction("配置机架");
    QAction *deleteBF = new QAction("删除机架");
    menu.addAction(cfgBF);
    menu.addAction(deleteBF);
    QPoint point(event->screenPos().x(), event->screenPos().y());
    QObject::connect(cfgBF, &QAction::triggered, this, [=](){
       emit(cfgBodyFrameItemSignal(this->bodyFrameItem.getBodyFrameItemID()));
    });
    connect(deleteBF, &QAction::triggered, this, [=](){
       emit(deleteBodyFrameItemSignal(this->bodyFrameItem.getBodyFrameItemID()));
    });
    menu.exec(point);
}

bool BodyFrameGraphicsItem::getHasSet() const
{
    return hasSet;
}

std::shared_ptr<QGraphicsItemGroup> BodyFrameGraphicsItem::getCollidingGroup() const
{
    return collidingGroup;
}

void BodyFrameGraphicsItem::computeLineToBus()
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


void BodyFrameGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsItem::mousePressEvent(event);
}

void BodyFrameGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    if(scene()->collidingItems(this).isEmpty()){
//        previousPos = pos();
//        return QGraphicsItem::mouseMoveEvent(event);
//    }
//    else{
//        //qDebug() << "pos" << pos() <<  "eventPos" << event->scenePos();
//        setPos(previousPos);
//        qDebug() << "colliding";
//    }
    return QGraphicsItem::mouseMoveEvent(event);
//=======

//    //    if(selectedItem != nullptr){//如果已经赋值
//    ////        QRectF collidingDetectArea = selectedItem->boundingRect().adjusted(-5,0,5,0);
////            collidingGroup->addToGroup(this);
////            if(scene()->collidingItems(this).isEmpty() == false && hasSet == false){
////                qDebug()<<"发生了碰撞";
//////                QPointF currentPos = mouseEvent->scenePos();
//////                qDebug() << currentPos;
//////                QPointF lastPos = mouseEvent->scenePos();
//////                qDebug() << lastPos;
//////                QPointF delta = currentPos - lastPos;
//////                QGraphicsItemGroup* group = new QGraphicsItemGroup();
////                for(QGraphicsItem* item : scene()->collidingItems(this)){
////                    if(dynamic_cast<BodyFrameGraphicsItem*>(item)){
////                          BodyFrameGraphicsItem* needMovementItem = dynamic_cast<BodyFrameGraphicsItem*>(item);
////                          collidingGroup->addToGroup(needMovementItem);
////                    }
////                }

//////                collidingGroup = std::shared_ptr<QGraphicsItemGroup>(group);
////                hasSet = true;
////                scene()->addItem(collidingGroup.get());
////                collidingGroup->setPos(event->scenePos());

//////                group->setPos(event->scenePos());
//////                lastPos = currentPos;
//////                qDebug()<< currentPos <<lastPos;
////        }else{
////              collidingGroup->setPos(event->scenePos());
////        }
//    //  }



//    if(scene()->collidingItems(this).isEmpty()){
//        previousPos = pos();
//        return QGraphicsItem::mouseMoveEvent(event);
//    }
//    else{
//        //qDebug() << "pos" << pos() <<  "eventPos" << event->scenePos();
//        setPos(previousPos);
//        qDebug() << "colliding";
//    }
//    return QGraphicsItem::mouseMoveEvent(event);


//>>>>>>> 22bfd3a200ec5ce734fa2d3404779f43f0d696da
}


void BodyFrameGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    computeLineToBus();
//    collidingGroup = nullptr;
//    hasSet = false;
    return QGraphicsItem::mouseReleaseEvent(event);
}

void BodyFrameGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //TODO. 打开机架，显示机架内部结构
    qDebug() << "打开机架";
    emit enterInBodyFrame(this->bodyFrameItem.getBodyFrameItemID());
    return QGraphicsItem::mouseDoubleClickEvent(event);
}

void BodyFrameGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QToolTip::showText(QCursor::pos(), QString("机架%1").arg(this->bodyFrameItem.getBodyFrameItemID()));
    return QGraphicsItem::hoverEnterEvent(event);
}

QVariant BodyFrameGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //防止移除到总线范围外
    if(change == QGraphicsItem::ItemPositionChange && this->scene()){
        QPointF newPos = value.toPointF();
        QRectF limitRec(500, 0, 4000 - this->boundingRect().height(), 2470 - this->boundingRect().width());
        if(limitRec.contains(newPos) == false){
            newPos.setX(qMin(limitRec.right(), qMax(newPos.x(), limitRec.left())));
            newPos.setY(qMin(limitRec.bottom(), qMax(newPos.y(), limitRec.top())));
            return newPos;
        }
    }

//    if ((change == ItemPositionChange || change == ItemPositionHasChanged) && scene()) // 控件发生移动
//    {
//        QPointF newPos = value.toPointF();
//        QRectF rect(0, 0, scene()->width(), scene()->height());

//        QList<QGraphicsItem *> list = collidingItems();//碰撞列表
//        if(list.size() > 0)
//        {
//            QGraphicsItem * otherItem = list.first();
//            QRectF otherItemRect = otherItem->boundingRect();
//            QRectF otherRect = QRectF(otherItem->x(),otherItem->y(),otherItemRect.width(),otherItemRect.height());

//            if(otherRect.contains(newPos))//左上角
//            {
//                QPointF tempPoint = otherRect.bottomRight() - newPos;
//                if(abs(tempPoint.x()) > abs(tempPoint.y()))
//                {
//                    newPos.setY(otherItem->y() + otherRect.height() + 2);
//                }
//                else
//                {
//                    newPos.setX(otherItem->x() + otherRect.width() + 2);
//                }
//                this->setPos(newPos);
//                return newPos;
//            }

//            QRectF thisRectF = boundingRect();
//            QPointF nowPos = QPointF(newPos.x() + thisRectF.width(),newPos.y());
//            if(otherRect.contains(nowPos))//右上角
//            {
//                QPointF tempPoint = otherRect.bottomLeft() - nowPos;
//                if(abs(tempPoint.x()) > abs(tempPoint.y()))
//                {
//                    newPos.setY(otherItem->y() + otherRect.height() + 2);
//                }
//                else
//                {
//                    newPos.setX(otherItem->x() - thisRectF.width() - 2);
//                }
//                this->setPos(newPos);
//                return newPos;
//            }

//            nowPos = QPointF(newPos.x(),newPos.y() + thisRectF.height());
//            if(otherRect.contains(nowPos))//左下角
//            {
//                QPointF tempPoint = otherRect.topRight() - nowPos;
//                if(abs(tempPoint.x()) > abs(tempPoint.y()))
//                {
//                    newPos.setY(otherItem->y() - thisRectF.height() - 2);
//                }
//                else
//                {
//                    newPos.setX(otherItem->x() + otherRect.width() + 2);
//                }
//                this->setPos(newPos);
//                return newPos;
//            }

//            nowPos = QPointF(newPos.x() + thisRectF.width(),newPos.y() + thisRectF.height());
//            if(otherRect.contains(nowPos))//右下角
//            {
//                QPointF tempPoint = otherRect.topLeft() - nowPos;
//                if(abs(tempPoint.x()) > abs(tempPoint.y()))
//                {
//                    newPos.setY(otherItem->y() - thisRectF.height() - 2);
//                }
//                else
//                {
//                    newPos.setX(otherItem->x() - otherRect.width() - 2);
//                }
//                this->setPos(newPos);
//                return newPos;
//            }
//        }
//    }
    return QGraphicsItem::itemChange(change, value);
}

QTreeWidgetItem *BodyFrameGraphicsItem::getTreeWidgetItem() const
{
    return treeWidgetItem;
}

void BodyFrameGraphicsItem::setTreeWidgetItem(QTreeWidgetItem *newTreeWidgetItem)
{
    treeWidgetItem = newTreeWidgetItem;
}

const BodyFrameToBusLineItem &BodyFrameGraphicsItem::getToAx() const
{
    return toAx;
}

const BodyFrameToBusLineItem &BodyFrameGraphicsItem::getToBy() const
{
    return toBy;
}

const BodyFrameToBusLineItem &BodyFrameGraphicsItem::getToBx() const
{
    return toBx;
}

const BodyFrameToBusLineItem &BodyFrameGraphicsItem::getToAy() const
{
    return toAy;
}


