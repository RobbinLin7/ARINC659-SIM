#ifndef DATAFLOW_H
#define DATAFLOW_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include "bodyFrameGraphicsItem.h"

class DataFlow:public QObject, public QGraphicsItem

{
    Q_OBJECT
public:
    DataFlow(BodyFrameGraphicsItem* from, BodyFrameGraphicsItem* to, QGraphicsItem *parent = nullptr);
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget = nullptr);
    // overwrite shape()
    QPainterPath shape() const;

private slots:
    void update1();

private:
    qreal m_offset;
    BodyFrameGraphicsItem* from = nullptr;
    BodyFrameGraphicsItem* to = nullptr;
//    QPointF points[4] = {
//        QPointF(100.0, 100.0) + QPointF(200.0, 200.0),
//        QPointF(100.0, 300.0) + QPointF(200.0, 200.0),
//        QPointF(300.0, 300.0) + QPointF(200.0, 200.0),
//        QPointF(300.0, 100.0) + QPointF(200.0, 200.0)
//    };
    QPointF AX_dataflow_points[4];
    QPointF AY_dataflow_points[4];
    QPointF BX_dataflow_points[4];
    QPointF BY_dataflow_points[4];

    void updatePoints();
    void drawDataFlow(QPainter *painter, QPointF* points, unsigned int size);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
};

#endif // DATAFLOW_H
