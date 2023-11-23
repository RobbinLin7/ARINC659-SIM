#include "innerbodyframescene.h"
#include "deviceModel/modulecfgwidget.h"
#include <QRgb>
#include <QMenu>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
InnerBodyFrameScene::InnerBodyFrameScene(BodyFrame& bodyFrame, QObject *parent)
    : QGraphicsScene{parent},
      bodyFrame(bodyFrame)
{
    for(auto module : bodyFrame.getModules()){
        std::shared_ptr<LRMGraphicsItem> myItem(new LRMGraphicsItem(module.second));
        moduleGraphicItems.insert(module.second.getModuleNumber(), myItem);
        myItem->setPos(100,100);
        myItem->setParent(this);
        this->addItem(myItem.get());
        connect(myItem.get(), &LRMGraphicsItem::cfgModuleSignal, this, &InnerBodyFrameScene::cfgModuleSlot);
        connect(myItem.get(), &LRMGraphicsItem::deleteModuleSignal, this, &InnerBodyFrameScene::deleteModuleSlot);
    }
    //for(int i = 0; i < bodyFrame.getModules(); i++)
    //std::shared_ptr<LRMGraphicsItem> myItem(new LRMGraphicsItem());
    //modules.insert()
    //LRMGraphicsItem* myItem = new LRMGraphicsItem();
}

//InnerBodyFrameScene::InnerBodyFrameScene()
//{
//    //LRMGraphicsItem* myItem = new LRMGraphicsItem();
//    //myItem->setParent(this);
//    //myItem->setPos(1000,2300);
//    //this->addItem(myItem);
//}

void InnerBodyFrameScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //if(QCursor::pos)
    QPointF mousePos = event->scenePos();
    QGraphicsItem *item = itemAt(mousePos, QTransform());
    if(item){
        LRMGraphicsItem* lrmItem = dynamic_cast<LRMGraphicsItem*>(item);
        lrmItem->contextMenuEvent(event);
    }
    else{
        QMenu menu;
        QAction *exitBF = new QAction("退出机架");
        menu.addAction(exitBF);
        connect(exitBF, &QAction::triggered, this, [=](){
            emit exitBodyFrameSignal();
        });
        menu.exec(QCursor::pos());
        return QGraphicsScene::contextMenuEvent(event);
    }
}

void InnerBodyFrameScene::cfgModuleSlot(uint moduleId)
{
    Module& module = bodyFrame.getModule(moduleId);
    ModuleCfgWidget *hm = new ModuleCfgWidget(module);
    connect(hm, &ModuleCfgWidget::saveModuleSignal, this, [=](const Module& module){
        bodyFrame.modifyModule(module);
    });
    hm->setWindowFlag(Qt::Dialog);
    hm->show();
}

void InnerBodyFrameScene::deleteModuleSlot(uint moduleId)
{
    moduleGraphicItems.remove(moduleId);
    bodyFrame.deleteModule(moduleId);
}
