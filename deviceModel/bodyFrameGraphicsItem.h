#ifndef BODYFRAMEGRAPHICSITEM_H
#define BODYFRAMEGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QTreeWidgetItem>
#include <QImage>
#include <QSize>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <memory>
#include "data/bodyframeitem.h"

class BodyFrameGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    ~BodyFrameGraphicsItem();
    BodyFrameGraphicsItem(BodyFrameItem bodyFrameItem, QObject* parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

//    void setBodyFrameID(const uint &id);

//    uint getBodyFrameID()const{ return myBodyFrameID;}

    QTreeWidgetItem *getTreeWidgetItem() const;
    void setTreeWidgetItem(QTreeWidgetItem *newTreeWidgetItem);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:

    QRectF imageRect;
    //获取机架图片大小
    QSize bodyFrameImageSize();

//    uint myBodyFrameID;             //机架号

    uint myHardwareModelNum;

//    uint arbitrationStepDuration;   //主/后备仲裁步进时长

//    uint timeCalibrationFactor;     //时间标定因子

//    uint majorVersionNumber;        //主版本号

//    uint subVersionNumber;          //次版本号

    QTreeWidgetItem* treeWidgetItem = nullptr;

    BodyFrameItem bodyFrameItem;

signals:

    void cfgBodyFrameItemSignal(uint frameId);

    void deleteBodyFrameItemSignal(uint frameId);

};

#endif // BODYFRAMEGRAPHICSITEM_H
