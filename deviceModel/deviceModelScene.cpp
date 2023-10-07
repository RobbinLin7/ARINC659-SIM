#include "deviceModelScene.h"

#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>
//#include "arrow.h"
DeviceModelScene::DeviceModelScene():
    Ax("Ax", QPoint(500, 2470), QPoint(4500, 2470)),
    Ay("Ay", QPoint(500, 2490), QPoint(4500, 2490)),
    Bx("Bx", QPoint(500, 2510), QPoint(4500, 2510)),
    By("By", QPoint(500, 2530), QPoint(4500, 2530))
{
    Ax.setParent(this);
    Ax.setPos(2500, 2470);
    this->addItem(&Ax);
    Ay.setParent(this);
    Ay.setPos(2500, 2490);
    this->addItem(&Ay);
    Bx.setParent(this);
    Bx.setPos(2500, 2510);
    this->addItem(&Bx);
    By.setParent(this);
    By.setPos(2500, 2530);
    this->addItem(&By);
    this->setBackgroundBrush(QBrush(Qt::gray));

}
bool DeviceModelScene::addBodyFrameItem(std::shared_ptr<BodyFrameGraphicsItem> item)
{
    if(item == nullptr) return false;

//    qDebug() << this->width();
    //qDebug() << "width" << this->width() << "height" << this->height();
    //this->addLine(QLineF(0, 600, 1800, 600), QPen());
    item->setParent(this);
    connect(item.get(), &BodyFrameGraphicsItem::cfgBodyFrameItemSignal, this, &DeviceModelScene::cfgBodyFrameItemSlot);
    connect(item.get(), &BodyFrameGraphicsItem::deleteBodyFrameItemSignal, this, &DeviceModelScene::deleteBodyFrameItemSlot);
    item->setPos(QPoint(550, 2000));
    qreal width = item->boundingRect().width() ;
    qreal height = item->boundingRect().height();
//    this->addLine(250 + width / 5,item->y() + height, 250 + width / 5, 700);
//    this->addLine(250 + width * 2 / 5,item->y() + height, 250 + width * 2 / 5, 725);
//    this->addLine(250 + width * 3 / 5,item->y() + height, 250 + width * 3 / 5, 750);
//    this->addLine(250 + width * 4 / 5,item->y() + height, 250 + width * 4 / 5, 775);
    //this->addLine(item->x() + item->boundingRect().width() / 5, item->y() + item->boundingRect().height(), item->x() + item->boundingRect().width() / 5, 2470);
    this->addItem(item.get());
    return true;
}

const BusGraphicsItem *DeviceModelScene::getAx() const
{
    return &Ax;
}

const BusGraphicsItem *DeviceModelScene::getAy() const
{
    return &Ay;
}

const BusGraphicsItem *DeviceModelScene::getBx() const
{
    return &Bx;
}

const BusGraphicsItem *DeviceModelScene::getBy() const
{
    return &By;
}


void DeviceModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DeviceModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DeviceModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


void DeviceModelScene::cfgBodyFrameItemSlot(uint bodyFrameId)
{
    emit(cfgBodyFrameItemSignal(bodyFrameId));
}

void DeviceModelScene::deleteBodyFrameItemSlot(uint bodyFrameId)
{
//    this->bodyFrameItemMap.remove(bodyFrameId);
//    flag[bodyFrameId] = false;
//    bodyFrameItemMap.value(bodyFrameId)->deleteLater();
    //    emit(deleteBodyFrameSignal(bodyFrameId));
}
