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
    QTreeWidgetItem *getTreeWidgetItem() const;
    void setTreeWidgetItem(QTreeWidgetItem *newTreeWidgetItem);

protected:

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:

    uint myHardwareModelNum;

    QTreeWidgetItem* treeWidgetItem = nullptr;

    BodyFrameItem bodyFrameItem;

    const QImage img;

    QPointF m_pos;
    QPointF m_pressedPos;
    QPointF m_startPos;

signals:

    void cfgBodyFrameItemSignal(uint frameId);

    void deleteBodyFrameItemSignal(uint frameId);


    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BODYFRAMEGRAPHICSITEM_H
