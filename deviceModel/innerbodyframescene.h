#ifndef INNERBODYFRAMESCENE_H
#define INNERBODYFRAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "data/bodyframe.h"
#include "deviceModel/lrmgrphicsitem.h"

class InnerBodyFrameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit InnerBodyFrameScene(const BodyFrame& bodyFrame, QObject *parent = nullptr);
    explicit InnerBodyFrameScene();
};

#endif // INNERBODYFRAMESCENE_H
