#ifndef BUSGRAPHICSITEM_H
#define BUSGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QAction>
#include "dialog/faultinjectdialog.h"

class BusGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public: 
    BusGraphicsItem(const QString& name, const QPoint& from, const QPoint& to, QGraphicsItem* parent = nullptr);
    BusGraphicsItem(const QString& name, const QLineF& line, QGraphicsItem* parent = nullptr);
private:
    bool error = false;
    QString name;
    QLineF line;
    qreal penWidth = 5 ;
    QAction *faultInjectAction = nullptr;
    QAction *cfgBF = nullptr;
    FaultInjectDialog::ErrorType errorType;
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // BUSGRAPHICSITEM_H
