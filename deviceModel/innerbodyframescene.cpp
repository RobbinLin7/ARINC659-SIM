#include "innerbodyframescene.h"

InnerBodyFrameScene::InnerBodyFrameScene(const BodyFrame& bodyFrame, QObject *parent)
    : QGraphicsScene{parent}
{
    LRMGraphicsItem* myItem = new LRMGraphicsItem();
    myItem->setPos(100,100);
    this->addItem(myItem);
}

InnerBodyFrameScene::InnerBodyFrameScene()
{
    LRMGraphicsItem* myItem = new LRMGraphicsItem();
    myItem->setPos(1000,2300);
    this->addItem(myItem);
}
