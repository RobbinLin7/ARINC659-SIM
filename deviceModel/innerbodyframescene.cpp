#include "innerbodyframescene.h"
#include "deviceModel/modulecfgwidget.h"
#include "deviceModel/bodyFrameCfgWidget.h"
#include "ui_bodyFrameCfgWidget.h"
#include <QRgb>
#include <QMenu>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
InnerBodyFrameScene::InnerBodyFrameScene(BodyFrame& bodyFrame, QObject *parent)
    : QGraphicsScene{parent},
      bodyFrame(bodyFrame),
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

    positionSet.insert(500);
    positionSet.insert(4000);
    for(auto module : bodyFrame.getModules()){
        std::shared_ptr<LRMGraphicsItem> myItem(new LRMGraphicsItem(this->getAx(),this->getAy(),this->getBx(),this->getBy(),module.second));
        moduleGraphicItems.insert(module.second.getModuleNumber(), myItem);
        myItem->setPos(550,2200);
        myItem->setParent(this);
        this->addIrmGraphicsItem(myItem.get());
        connect(myItem.get(), &LRMGraphicsItem::cfgModuleSignal, this, &InnerBodyFrameScene::cfgModuleSlot);
        connect(myItem.get(), &LRMGraphicsItem::deleteModuleSignal, this, &InnerBodyFrameScene::deleteModuleSlot);
    }
    //for(int i = 0; i < bodyFrame.getModules(); i++)
    //std::shared_ptr<LRMGraphicsItem> myItem(new LRMGraphicsItem());
    //modules.insert()
    //LRMGraphicsItem* myItem = new LRMGraphicsItem();
}


bool InnerBodyFrameScene::addIrmGraphicsItem(LRMGraphicsItem* item)
{
    if(item == nullptr) return false;
    item->setParent(this);


    auto left = positionSet.cbegin();
    auto right = positionSet.cbegin();
    ++right;
    for(;;){
        if(left ==  positionSet.cend()) break;
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
//    item->setPos(QPointF(1000, 2200));
    qreal width = item->boundingRect().width() ;
    qreal height = item->boundingRect().height();
//    this->addLine(250 + width / 5,item->y() + height, 250 + width / 5, 700);
//    this->addLine(250 + width * 2 / 5,item->y() + height, 250 + width * 2 / 5, 725);
//    this->addLine(250 + width * 3 / 5,item->y() + height, 250 + width * 3 / 5, 750);
//    this->addLine(250 + width * 4 / 5,item->y() + height, 250 + width * 4 / 5, 775);
//    this->addLine(item->x() + item->boundingRect().width() / 5, item->y() + item->boundingRect().height(), item->x() + item->boundingRect().width() / 5, 2470);
    this->addItem(item);
//    item->setPos(QPointF(1000,2200));
    return true;
}

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
        QAction *addModule = new QAction("新增模块");
        QAction* cfgDF = new QAction("配置帧");
        menu.addAction(exitBF);
        menu.addAction(addModule);
        menu.addAction(cfgDF);
        connect(exitBF, &QAction::triggered, this, [=](){
            emit exitBodyFrameSignal();
        });
        connect(addModule,&QAction::triggered,this, [=](){
            BodyFrameCfgWidget* newWidget = new BodyFrameCfgWidget(this->bodyFrame);
            newWidget->ui->tab_2->setWindowFlag(Qt::Dialog);
            QPushButton* okPushButton = new QPushButton("确定", newWidget->ui->tab_2);
            newWidget->ui->widget_3->layout()->addWidget(okPushButton);
            newWidget->ui->tab_2->show();
            connect(okPushButton,&QPushButton::clicked,this,[=](){
                emit innerAddMoudleSignal()
            });
        });
        connect(cfgDF, &QAction::triggered, this, [=](){
           BodyFrameCfgWidget *widget = new BodyFrameCfgWidget(bodyFrame);
           widget->ui->tab_3->setWindowFlags(Qt::Dialog);
           QPushButton *okPushButton = new QPushButton("确定", widget->ui->tab_3);
           //widget->ui->horizontalLayout_4->addWidget(okPushButton);
           if(widget->ui->widget_4->layout() != nullptr){
               widget->ui->widget_4->layout()->addWidget(okPushButton);
           }
           else qDebug() << "meiyou layout";
           widget->ui->tab_3->show();
        });
        menu.exec(QCursor::pos());
        return QGraphicsScene::contextMenuEvent(event);
    }
}

const BusGraphicsItem* InnerBodyFrameScene::getBy() const
{
    return &By;
}

const BusGraphicsItem* InnerBodyFrameScene::getBx() const
{
    return &Bx;
}

const BusGraphicsItem* InnerBodyFrameScene::getAy() const
{
    return &Ay;
}

const BusGraphicsItem* InnerBodyFrameScene::getAx() const
{
    return &Ax;
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

//void InnerBodyFrameScene::addModuleSlot(BodyFrame& bodyFrame)
//{
//    BodyFrameCfgWidget *dialog = new BodyFrameCfgWidget(bodyFrame);
//    dialog->ui->tab_2->setWindowFlag(Qt::Dialog);
//    dialog->ui->tab_2->show();

////    connect(dialog, &BodyFrameCfgWidget::saveBodyFrameItemSignal, this, [=](const Module& module){
////        bodyFrame.modifyModule(module);
////    });
////    dialog->setWindowFlag(Qt::Dialog);
////    dialog->show();
//}

std::set<int> InnerBodyFrameScene::getPositionSet() const
{
    return positionSet;
}


//InnerBodyFrameScene::InnerBodyFrameScene()
//{
//    LRMGraphicsItem* myItem = new LRMGraphicsItem();
//    myItem->setPos(1000,2300);
//    this->addItem(myItem);
//}
