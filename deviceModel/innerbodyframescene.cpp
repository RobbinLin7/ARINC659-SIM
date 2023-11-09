#include "innerbodyframescene.h"

InnerBodyFrameScene::InnerBodyFrameScene(const BodyFrame& bodyFrame, QObject *parent)
    : QGraphicsScene{parent}
{
    LRMGrphicsItem* myItem = new LRMGrphicsItem();
    myItem->setPos(100,100);
    this->addItem(myItem);
}

InnerBodyFrameScene::InnerBodyFrameScene()
{
    LRMGrphicsItem* myItem = new LRMGrphicsItem();
    myItem->setPos(1000,2300);
    this->addItem(myItem);
}
