#ifndef INNERBODYFRAMESCENE_H
#define INNERBODYFRAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QMap>
#include "data/bodyframe.h"
#include "deviceModel/lrmgrphicsitem.h"
#include "deviceModel/busgraphicsitem.h"
#include <memory>
#include "data/module.h"

class InnerBodyFrameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit InnerBodyFrameScene(BodyFrame& bodyFrame, QObject *parent = nullptr);
    //explicit InnerBodyFrameScene();
    bool addIrmGraphicsItem(LRMGraphicsItem* item);


    std::set<int> getPositionSet() const;

    const BusGraphicsItem* getAx() const;

    const BusGraphicsItem* getAy() const;

    const BusGraphicsItem* getBx() const;

    const BusGraphicsItem* getBy() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
signals:
    void exitBodyFrameSignal();
    void innerAddMoudleSignal(QList<Module>);
    void modifyBodyFrameSignal(const BodyFrame&);
private:
    QMap<uint, std::shared_ptr<LRMGraphicsItem>> moduleGraphicItems;
    BodyFrame& bodyFrame;
    BusGraphicsItem Ax;
    BusGraphicsItem Ay;
    BusGraphicsItem Bx;
    BusGraphicsItem By;
    std::set<int> positionSet; //记录所有机架的水平位置
private slots:
    void cfgModuleSlot(uint moduleId);
    void deleteModuleSlot(uint moduleId);
//void addModuleSlot(BodyFrame& bodyFrame);
};

#endif // INNERBODYFRAMESCENE_H
