#include "deviceModelScene.h"

#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>

DeviceModelScene::DeviceModelScene()
{

}

uint DeviceModelScene::addBodyFrameItem()
{
    uint minIdUnused = getMinIdUnused();
    if(minIdUnused > 0){
        std::shared_ptr<BodyFrameItem> item = std::make_shared<BodyFrameItem>(minIdUnused, this);
        connect(item.get(), &BodyFrameItem::cfgBodyFrameItemSignal, this, &DeviceModelScene::cfgBodyFrameItemSlot);
        connect(item.get(), &BodyFrameItem::deleteBodyFrameItemSignal, this, &DeviceModelScene::deleteBodyFrameItemSlot);
        flag[minIdUnused] = true;
        this->bodyFrameItemMap.insert(minIdUnused, item);
        item->setPos(QPointF(10, 10));
        item->setBodyFrameID(minIdUnused);
        this->addItem(item.get());
        return minIdUnused;
    }
    else{
        qDebug() << QString(tr("bodyFrameId 分配失败"));
        return 0;
    }
}


uint DeviceModelScene::getMinIdUnused()
{
    for(uint i = 1; i <= maxBodyFrameId; i++){
        if(flag[i] == false){
            return i;
        }
    }
    return 0;
}

void DeviceModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    qDebug() << "mousePressEvent";
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

//void DeviceModelScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
//{

//    const QList<QGraphicsItem *> items = selectedItems();

//    if(items.size() <= 0)
//    {
//        return;
//    }else
//    {
//        qDebug() << "selected :" << items.size();
//        this->selectedItem = (BodyFrameItem *)items.at(0);
//    }
//    QMenu menu;

//    QAction *cfgBF = new QAction("配置机架", this);
//    QAction *deleteBF = new QAction("删除机架", this);

//    menu.addAction(cfgBF);
//    menu.addAction(deleteBF);

//    QPoint point(event->screenPos().x(), event->screenPos().y());

//    connect(cfgBF, SIGNAL(triggered()), this, SLOT(cfgBFSlot()));

//    menu.exec(point);
//}


void DeviceModelScene::cfgBodyFrameItemSlot(uint bodyFrameId)
{
    emit(cfgBodyFrameItemSignal(bodyFrameId));
}

void DeviceModelScene::deleteBodyFrameItemSlot(uint bodyFrameId)
{
    this->bodyFrameItemMap.remove(bodyFrameId);
    flag[bodyFrameId] = false;
    bodyFrameItemMap.value(bodyFrameId)->deleteLater();
    emit(deleteBodyFrameItemSignal(bodyFrameId));
}
