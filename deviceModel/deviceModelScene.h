#ifndef DEVICEMODELSCENE_H
#define DEVICEMODELSCENE_H

#include <QGraphicsScene>

#include "bodyFrameItem.h"

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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private slots:

    void cfgBFSlot();

private:
    BodyFrameItem *selectedItem;

signals:
    void cfgBFSignal(BodyFrameItem *selectedItem);
};

#endif // DEVICEMODELSCENE_H
