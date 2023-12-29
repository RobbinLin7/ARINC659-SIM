#ifndef DEVICEMODELSCENE_H
#define DEVICEMODELSCENE_H
#include <QGraphicsScene>
#include <memory>
#include "bodyFrameGraphicsItem.h"
#include "deviceModel/bodyFrameCfgWidget.h"
#include "deviceModel/busgraphicsitem.h"
#include "deviceModel/dataflow.h"
#include "arrow.h"
#include <memory>
#include <QMap>
#include <set>
#include <map>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE


class DeviceModelScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum DeviceModel{BodyFrame};
    DeviceModelScene();
    bool addBodyFrameItem(BodyFrameGraphicsItem*);
    void deleteBodyFrameItem(int x);
    void addFrame(std::shared_ptr<BodyFrameGraphicsItem> from, std::shared_ptr<BodyFrameGraphicsItem> to);
    void setAx(int, int, int, int);
    void setAy(int, int, int, int);
    void setBx(int, int, int, int);
    void setBy(int, int, int, int);
    const BusGraphicsItem* getAx() const;
    const BusGraphicsItem* getAy() const;
    const BusGraphicsItem* getBx() const;
    const BusGraphicsItem *getBy() const;

    void simulationStart();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private slots:

    void cfgBodyFrameItemSlot(uint);
    void deleteBodyFrameItemSlot(uint, qreal);


private:
    static const int maxBodyFrameId = 15;
    bool flag[maxBodyFrameId];
    std::shared_ptr<BodyFrameGraphicsItem> selectedItem =nullptr ;
    BusGraphicsItem Ax;
    BusGraphicsItem Ay;
    BusGraphicsItem Bx;
    BusGraphicsItem By;
    std::map<int, int> positionSet; //记录所有机架的水平位置
    std::map<std::pair<BodyFrameGraphicsItem*, BodyFrameGraphicsItem*>, std::shared_ptr<DataFlow>> dataflows;
    //QMap<uint, std::shared_ptr<BodyFrameItem>> bodyFrameItemMap;

signals:
//    void cfgBFSignal(BodyFrameItem *selectedItem);
    void cfgBodyFrameItemSignal(uint);
    void deleteBodyFrameSignal(uint);

};

#endif // DEVICEMODELSCENE_H
