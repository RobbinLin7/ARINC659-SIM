#ifndef BODYFRAMEITEM_H
#define BODYFRAMEITEM_H

#include <QGraphicsItem>
#include <QImage>
#include <QSize>
#include <QDebug>
#include <QMenu>
#include <QAction>
//#include "bodyFrame.h"
class BodyFrameItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    BodyFrameItem(uint frameId, QObject* parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void setBodyFrameID(const uint &id);

    uint getBodyFrameID(){ return myBodyFrameID;}

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

    uint myBodyFrameID;

    uint myHardwareModelNum;

    //BodyFrame *myBodyFrame;

signals:

    void cfgBodyFrameItemSignal(uint frameId);

};

#endif // BODYFRAMEITEM_H
