#ifndef BUSGRAPHICSITEM_H
#define BUSGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QAction>

class BusGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    BusGraphicsItem(const QString& name, const QPoint& from, const QPoint& to, QGraphicsItem* parent = nullptr);
    BusGraphicsItem(const QString& name, const QLineF& line, QGraphicsItem* parent = nullptr);
private:
    QString name;
    QLineF line;
    qreal penWidth = 5;
    QAction *faultInjectAction = nullptr;
    QAction *cfgBF = nullptr;
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    // QGraphicsItem interface
public:
    //bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const;
private slots:
    void on_faultInjectAction_triggered();
};

#endif // BUSGRAPHICSITEM_H
