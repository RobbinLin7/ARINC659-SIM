#ifndef DEVICEMODELSCENE_H
#define DEVICEMODELSCENE_H
#include <QGraphicsScene>
#include <memory>
#include "bodyFrameItem.h"
#include "deviceModel/bodyFrame.h"
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
    uint addBodyFrameItem();
    void deleteBodyFrameItem();
    uint getMinIdUnused();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private slots:

    void cfgBodyFrameItemSlot(uint);
    void deleteBodyFrameItemSlot(uint);

private:
    static const int maxBodyFrameId = 1000;
    bool flag[maxBodyFrameId];
    BodyFrameItem *selectedItem;
    QMap<uint, std::shared_ptr<BodyFrameItem>> bodyFrameItemMap;

signals:
//    void cfgBFSignal(BodyFrameItem *selectedItem);
    void cfgBodyFrameItemSignal(uint);
    void deleteBodyFrameItemSignal(uint);

};

#endif // DEVICEMODELSCENE_H
