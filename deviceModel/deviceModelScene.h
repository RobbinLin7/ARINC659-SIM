#ifndef DEVICEMODELSCENE_H
#define DEVICEMODELSCENE_H
#include <QGraphicsScene>
#include <memory>
#include "bodyFrameGraphicsItem.h"
#include "deviceModel/bodyFrameCfgWidget.h"
#include "deviceModel/linegraphicsitem.h"
#include <memory>
#include <QMap>

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
    bool addBodyFrameItem(std::shared_ptr<BodyFrameGraphicsItem>);
    void deleteBodyFrameItem();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private slots:

    void cfgBodyFrameItemSlot(uint);
    void deleteBodyFrameItemSlot(uint);

private:
    static const int maxBodyFrameId = 15;
    bool flag[maxBodyFrameId];
    BodyFrameGraphicsItem *selectedItem;
    linegraphicsitem Ax;
    linegraphicsitem Ay;
    linegraphicsitem Bx;
    linegraphicsitem By;
    //QMap<uint, std::shared_ptr<BodyFrameItem>> bodyFrameItemMap;

signals:
//    void cfgBFSignal(BodyFrameItem *selectedItem);
    void cfgBodyFrameItemSignal(uint);
    void deleteBodyFrameSignal(uint);

};

#endif // DEVICEMODELSCENE_H
