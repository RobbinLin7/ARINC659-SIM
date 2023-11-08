#ifndef LRMGRPHICSITEM_H
#define LRMGRPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>

class LRMGrphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    LRMGrphicsItem();
};

#endif // LRMGRPHICSITEM_H
