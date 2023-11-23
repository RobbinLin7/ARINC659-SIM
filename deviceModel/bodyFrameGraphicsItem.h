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
#include "data/bodyframe.h"
#include "busgraphicsitem.h"
#include "bodyframetobuslineitem.h"

class BodyFrameGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    ~BodyFrameGraphicsItem();
    BodyFrameGraphicsItem(const BusGraphicsItem* Ax, const BusGraphicsItem* Ay, const BusGraphicsItem* Bx, const BusGraphicsItem* By,
                          BodyFrame bodyFrameItem, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    QTreeWidgetItem *getTreeWidgetItem() const;
    void setTreeWidgetItem(QTreeWidgetItem *newTreeWidgetItem);

    const BodyFrameToBusLineItem &getToAx() const;

    const BodyFrameToBusLineItem &getToAy() const;

    const BodyFrameToBusLineItem &getToBx() const;

    const BodyFrameToBusLineItem &getToBy() const;

protected:

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:

    uint myHardwareModelNum;

    QTreeWidgetItem* treeWidgetItem = nullptr;

    BodyFrame bodyFrameItem;

    const QImage img;

    QPointF m_pos;
    QPointF m_pressedPos;
    QPointF m_startPos;

    QPointF previousPos;

    BodyFrameToBusLineItem toAx;
    BodyFrameToBusLineItem toAy;
    BodyFrameToBusLineItem toBx;
    BodyFrameToBusLineItem toBy;

    void computeLineToBus();


signals:

    void cfgBodyFrameItemSignal(uint frameId);

    void deleteBodyFrameItemSignal(uint frameId);

    void enterInBodyFrame(uint frameId);


    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value)override;
};

#endif // BODYFRAMEGRAPHICSITEM_H
