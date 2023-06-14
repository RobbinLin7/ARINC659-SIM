#include "deviceModelScene.h"

#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>

DeviceModelScene::DeviceModelScene()
{

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

void DeviceModelScene::cfgBFSlot()
{
    qDebug() << "DeviceModelScene::cfgBFSlot()";

    emit cfgBFSignal(this->selectedItem);
}
