#include "busgraphicsitem.h"
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <iostream>
#include <QDebug>
#include "dialog/faultinjectdialog.h"

BusGraphicsItem::BusGraphicsItem(const QString& name, const QPoint &from, const QPoint &to, QGraphicsItem* parent):
    QGraphicsItem(parent),
    name(name),
    line(QLineF(from, to)){
    setZValue(0); //设置z值，z值越高越在上层
    setFlags(ItemIsSelectable | ItemIsMovable);

}

BusGraphicsItem::BusGraphicsItem(const QString& name, const QLineF &line, QGraphicsItem* parent):
    QGraphicsItem(parent),
    name(name),
    line(line){
    setZValue(0);
    setFlags(ItemIsSelectable | ItemIsMovable);
}


QRectF BusGraphicsItem::boundingRect() const
{
    return QRectF(-1 * line.dx() / 2, -1 * penWidth / 2, line.dx(), penWidth);
}

void BusGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setWidth(penWidth);
    painter->setPen(pen);
    painter->drawText(-1 * line.dx() / 2, 0, this->name);
    painter->drawLine(-1 * line.dx() / 2 + 20, 0, line.dx() / 2, 0);
    painter->save();
    painter->restore();
}

void BusGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    cfgBF = new QAction(QString("配置%1").arg(this->name));
    faultInjectAction = new QAction(QString("故障注入"));
    menu.addAction(cfgBF);
    menu.addAction(faultInjectAction);
    connect(faultInjectAction, &QAction::triggered, this, [=](){
        qDebug() << "hahah";
        FaultInjectDialog *dialog = new FaultInjectDialog();
        dialog->setWindowTitle(QString("%1故障注入").arg(this->name));
        dialog->setWindowFlag(Qt::Dialog);
        dialog->exec();
    });
    QPoint point(event->screenPos().x(), event->screenPos().y());
    menu.exec(point);
}

void BusGraphicsItem::on_faultInjectAction_triggered()
{
    qDebug() << "hahah";
    FaultInjectDialog *dialog = new FaultInjectDialog();
    dialog->setWindowFlag(Qt::Dialog);
    dialog->show();
}
