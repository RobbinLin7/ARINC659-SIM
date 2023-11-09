#ifndef LRMGRPHICSITEM_H
#define LRMGRPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QRectF>

class LRMGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    LRMGraphicsItem();
    QRectF getRect_bound() const;
    void setRect_bound(const QRectF &value);

    QRectF getRect_cpu() const;
    void setRect_cpu(const QRectF &value);

    QRectF getRect_659() const;
    void setRect_659(const QRectF &value);

private:
    QRectF rect_cpu;
    QRectF rect_659;
    QRectF rect_bound;
    // QGraphicsItem interface
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // LRMGRPHICSITEM_H
