#ifndef INNERBODYFRAMESCENE_H
#define INNERBODYFRAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "data/bodyframe.h"
#include "deviceModel/lrmgrphicsitem.h"
#include "deviceModel/busgraphicsitem.h"

class InnerBodyFrameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit InnerBodyFrameScene(const BodyFrame& bodyFrame, QObject *parent = nullptr);
//    explicit InnerBodyFrameScene();
    bool addIrmGraphicsItem(LRMGraphicsItem* item);


    std::set<int> getPositionSet() const;

private:
    BusGraphicsItem Ax;
    BusGraphicsItem Ay;
    BusGraphicsItem Bx;
    BusGraphicsItem By;
    std::set<int> positionSet; //记录所有机架的水平位置



};

#endif // INNERBODYFRAMESCENE_H
