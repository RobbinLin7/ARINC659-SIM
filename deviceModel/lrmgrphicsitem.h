#ifndef LRMGRPHICSITEM_H
#define LRMGRPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QRectF>
#include "data/module.h"

class LRMGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    LRMGraphicsItem(const Module& module);
    QRectF getRect_bound() const;
    void setRect_bound(const QRectF &value);

    QRectF getRect_cpu() const;
    void setRect_cpu(const QRectF &value);

    QRectF getRect_659() const;
    void setRect_659(const QRectF &value);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    QRectF rect_cpu;
    QRectF rect_659;
    QRectF rect_bound;
    Module module;

    // QGraphicsItem interface
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    void cfgModuleSignal(uint moduleId);
    void deleteModuleSignal(uint moduleId);
};

#endif // LRMGRPHICSITEM_H
