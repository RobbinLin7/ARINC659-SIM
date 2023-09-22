#include "deviceModelScene.h"

#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>

DeviceModelScene::DeviceModelScene()
{
    //qDebug()
    //qDebug() << "width" << this->width() << "height" << this->height();
    //this->addLine(QLineF(0, 0, this->width(), this->height()), QPen());
    //this->addLine(0, 100, 100, 100);
    //update();
    //this->addText("哈哈哈哈哈啊哈哈");
    //qDebug() << "haha";
    this->addLine(Ax);
    this->addLine(Ay);
    this->addLine(Bx);
    this->addLine(By);
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
    item->setPos(QPointF(10, 10));
    this->addItem(item.get());
    return true;
}

void DeviceModelScene::setAx(int x1, int y1, int x2, int y2){
    Ax.setPoints(QPoint(x1, y1), QPoint(x2, y2));
}

void DeviceModelScene::setAy(int x1, int y1, int x2, int y2)
{
    Ay.setPoints(QPoint(x1, y1), QPoint(x2, y2));
}

void DeviceModelScene::setBx(int x1, int y1, int x2, int y2)
{
    Bx.setPoints(QPoint(x1, y1), QPoint(x2, y2));
}

void DeviceModelScene::setBy(int x1, int y1, int x2, int y2)
{
    By.setPoints(QPoint(x1, y1), QPoint(x2, y2));
}

void DeviceModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    qDebug() << "mousePressEvent";
    qDebug() << QCursor::pos().x() << QCursor::pos().y();
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
