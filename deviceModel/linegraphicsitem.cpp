#include "linegraphicsitem.h"
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>


linegraphicsitem::linegraphicsitem(const QString& name, const QPoint &from, const QPoint &to):
    name(name),
    line(QLineF(from, to)){

}

linegraphicsitem::linegraphicsitem(const QString& name, const QLineF &line):
    name(name),
    line(line){}

QRectF linegraphicsitem::boundingRect() const
{
    qreal penWidth = 10;
    return QRectF(line.p1().x(), line.p1().y() - penWidth / 2, line.p2().x() - line.p1().x(), penWidth);
}

void linegraphicsitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawLine(line);
    QPoint textPoint(line.x1(), line.y1() - 10);
    painter->drawText(textPoint, name);
    painter->save();
    painter->restore();
}

void linegraphicsitem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *cfgBF = new QAction("配置");
    QAction *deleteBF = new QAction("删除");
    menu.addAction(cfgBF);
    menu.addAction(deleteBF);
    QPoint point(event->screenPos().x(), event->screenPos().y());
    menu.exec(point);
}
