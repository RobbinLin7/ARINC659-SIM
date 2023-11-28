#include "dataflow.h"
#include "dataflow.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QDebug>


DataFlow::DataFlow(BodyFrameGraphicsItem* from, BodyFrameGraphicsItem* to, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_offset(0),
      from(from),
      to(to)
{
    this->setZValue(1);
    updatePoints();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update1()));
    timer->start(100);
}

void DataFlow::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    updatePoints();
    drawDataFlow(painter, AX_dataflow_points, 4);
    drawDataFlow(painter, AY_dataflow_points, 4);
    drawDataFlow(painter, BX_dataflow_points, 4);
    drawDataFlow(painter, BY_dataflow_points, 4);
//    qreal penWidth = 3;
//    painter->save();
//    QPen pen1(Qt::yellow);
//    QVector<qreal> dashes;
//    qreal space = 1;
//    dashes << 2 << space << 2 << space;
//    pen1.setDashPattern(dashes);
//    pen1.setWidth(penWidth);
//    pen1.setJoinStyle(Qt::RoundJoin);    // MiterJoin, BevelJoin, RoundJoin
//    pen1.setCapStyle(Qt::RoundCap);      // FlatCap, SquareCap, RoundCap
//    pen1.setDashOffset(m_offset);
//    painter->setPen(pen1);
//    painter->drawPolyline(points, 4);
//    painter->restore();
}

QPainterPath DataFlow::shape() const
{

    QPainterPath path;
    path.moveTo(AX_dataflow_points[0]);
    path.lineTo(AX_dataflow_points[1]);
    path.lineTo(AX_dataflow_points[2]);
    path.lineTo(AX_dataflow_points[3]);

    //return path;
    QPainterPathStroker stroker;
    stroker.setWidth(10);
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setDashPattern(Qt::DashLine);
    return stroker.createStroke(path);
}

void DataFlow::update1()
{
    m_offset += 0.5;
    update();
}

void DataFlow::updatePoints()
{
    AX_dataflow_points[0] = QPointF(from->x() + from->boundingRect().width() / 5, from->y() + from->boundingRect().height());
    AX_dataflow_points[1] = QPointF(from->x() + from->boundingRect().width() / 5, from->getToAx().getBus()->y());
    AX_dataflow_points[2] = QPointF(to->x() + to->boundingRect().width() / 5, to->getToAx().getBus()->y());
    AX_dataflow_points[3] = QPointF(to->x() + to->boundingRect().width() / 5, to->y() + to->boundingRect().height());

    AY_dataflow_points[0] = QPointF(from->x() + from->boundingRect().width() * 2 / 5, from->y() + from->boundingRect().height());
    AY_dataflow_points[1] = QPointF(from->x() + from->boundingRect().width() * 2 / 5, from->getToAy().getBus()->y());
    AY_dataflow_points[2] = QPointF(to->x() + to->boundingRect().width() * 2 / 5, to->getToAy().getBus()->y());
    AY_dataflow_points[3] = QPointF(to->x() + to->boundingRect().width() * 2 / 5, to->y() + to->boundingRect().height());

    BX_dataflow_points[0] = QPointF(from->x() + from->boundingRect().width() * 3 / 5, from->y() + from->boundingRect().height());
    BX_dataflow_points[1] = QPointF(from->x() + from->boundingRect().width() * 3 / 5, from->getToBx().getBus()->y());
    BX_dataflow_points[2] = QPointF(to->x() + to->boundingRect().width() * 3 / 5, to->getToBx().getBus()->y());
    BX_dataflow_points[3] = QPointF(to->x() + to->boundingRect().width() * 3 / 5, to->y() + to->boundingRect().height());

    BY_dataflow_points[0] = QPointF(from->x() + from->boundingRect().width() * 4 / 5, from->y() + from->boundingRect().height());
    BY_dataflow_points[1] = QPointF(from->x() + from->boundingRect().width() * 4 / 5, from->getToBy().getBus()->y());
    BY_dataflow_points[2] = QPointF(to->x() + to->boundingRect().width() * 4 / 5, to->getToBy().getBus()->y());
    BY_dataflow_points[3] = QPointF(to->x() + to->boundingRect().width() * 4 / 5, to->y() + to->boundingRect().height());
}

void DataFlow::drawDataFlow(QPainter* painter, QPointF *points, unsigned int size)
{
    qDebug() << "draw";
    qreal penWidth = 3;
    painter->save();
    QPen pen1(Qt::yellow);
    QVector<qreal> dashes;
    qreal space = 1;
    dashes << 2 << space << 2 << space;
    pen1.setDashPattern(dashes);
    pen1.setWidth(penWidth);
    pen1.setJoinStyle(Qt::RoundJoin);    // MiterJoin, BevelJoin, RoundJoin
    pen1.setCapStyle(Qt::RoundCap);      // FlatCap, SquareCap, RoundCap
    pen1.setDashOffset(m_offset);
    painter->setPen(pen1);
    painter->drawPolyline(points, size);
    painter->restore();
}

QRectF DataFlow::boundingRect() const
{
    return QRectF(0, 0, AX_dataflow_points[2].x(), AX_dataflow_points[2].y());
}

