#include "deviceModelScene.h"

#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>

DeviceModelScene::DeviceModelScene():
    Ax("Ax", QPoint(200, 700), QPoint(1000, 700)),
    Ay("Ay", QPoint(200, 725), QPoint(1000, 725)),
    Bx("Bx", QPoint(200, 750), QPoint(1000, 750)),
    By("By", QPoint(200, 775), QPoint(1000, 775))
{
    Ax.setParent(this);
    this->addItem(&Ax);
    Ay.setParent(this);
    this->addItem(&Ay);
    Bx.setParent(this);
    this->addItem(&Bx);
    By.setParent(this);
    this->addItem(&By);

    this->setBackgroundBrush(QBrush(Qt::gray));
}
bool DeviceModelScene::addBodyFrameItem(std::shared_ptr<BodyFrameGraphicsItem> item)
{
    if(item == nullptr) return false;
//    this->addLine(0, 100, this->width(), 100);
//    qDebug() << this->width();
    item->setParent(this);
    connect(item.get(), &BodyFrameGraphicsItem::cfgBodyFrameItemSignal, this, &DeviceModelScene::cfgBodyFrameItemSlot);
    connect(item.get(), &BodyFrameGraphicsItem::deleteBodyFrameItemSignal, this, &DeviceModelScene::deleteBodyFrameItemSlot);
    item->setPos(QPointF(0, 0));
    this->addItem(item.get());

    return true;
}

void DeviceModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    //qDebug() << "mousePressEvent";
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
//    this->bodyFrameItemMap.remove(bodyFrameId);
//    flag[bodyFrameId] = false;
//    bodyFrameItemMap.value(bodyFrameId)->deleteLater();
//    emit(deleteBodyFrameSignal(bodyFrameId));
}
