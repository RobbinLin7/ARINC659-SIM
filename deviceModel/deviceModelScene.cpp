#include "deviceModelScene.h"

#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>
#include "dataflow.h"
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

    positionSet.insert(500);
    positionSet.insert(4000);

}
bool DeviceModelScene::addBodyFrameItem(std::shared_ptr<BodyFrameGraphicsItem> item)
{
    if(item == nullptr) return false;
    item->setParent(this);
    connect(item.get(), &BodyFrameGraphicsItem::cfgBodyFrameItemSignal, this, &DeviceModelScene::cfgBodyFrameItemSlot);
    connect(item.get(), &BodyFrameGraphicsItem::deleteBodyFrameItemSignal, this, &DeviceModelScene::deleteBodyFrameItemSlot);
//    if(positionSet.empty()){
//        item->setPos(QPoint(550, 2000));
//    }
//    else{

//    }
    auto left = positionSet.cbegin();
    auto right = positionSet.cbegin();
    ++right;
    for(;;){
        if(left == positionSet.cend()) break;
        if(*right - *left > item->boundingRect().width() + 50){
            if(*left != 500){
                item->setPos(QPoint(*left + 50 + item->boundingRect().width(), 2200));
                positionSet.insert(*left + 50 + item->boundingRect().width());
            }
            else{
                qDebug() << "left =" << *left + 10 + item->boundingRect().width() / 2;
                item->setPos(QPoint(*left + item->boundingRect().width() / 2, 2200));
                positionSet.insert(*left + item->boundingRect().width() / 2);
            }
            break;
        }
        ++left;
        ++right;
    }
    //item->setPos(QPoint(550, 2000));
//    qreal width = item->boundingRect().width() ;
//    qreal height = item->boundingRect().height();
//    this->addLine(250 + width / 5,item->y() + height, 250 + width / 5, 700);
//    this->addLine(250 + width * 2 / 5,item->y() + height, 250 + width * 2 / 5, 725);
//    this->addLine(250 + width * 3 / 5,item->y() + height, 250 + width * 3 / 5, 750);
//    this->addLine(250 + width * 4 / 5,item->y() + height, 250 + width * 4 / 5, 775);
    //this->addLine(item->x() + item->boundingRect().width() / 5, item->y() + item->boundingRect().height(), item->x() + item->boundingRect().width() / 5, 2470);
    this->addItem(item.get());
    return true;
}

void DeviceModelScene::deleteBodyFrameItem(int x)
{
    positionSet.erase(x);
}

void DeviceModelScene::addFrame(std::shared_ptr<BodyFrameGraphicsItem> from, std::shared_ptr<BodyFrameGraphicsItem> to)
{
    DataFlow* flow = new DataFlow(from.get(), to.get());
    dataflows[std::make_pair(from.get(), to.get())] = std::shared_ptr<DataFlow>(flow);
    //dataflows.insert(std::make_pair(from.get(), to.get()), std::shared_ptr<DataFlow>(flow));
    flow->setParent(this);
    this->addItem(flow);
    flow->update();
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
