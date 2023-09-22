#ifndef LINEGRAPHICSITEM_H
#define LINEGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>

class linegraphicsitem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    linegraphicsitem(const QString& name, const QPoint& from, const QPoint& to);
    linegraphicsitem(const QString& name, const QLineF& line);
private:
    QString name;
    QLineF line;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    // QGraphicsItem interface
public:
    //bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;
};

#endif // LINEGRAPHICSITEM_H
