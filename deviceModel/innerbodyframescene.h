#ifndef INNERBODYFRAMESCENE_H
#define INNERBODYFRAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "data/bodyframe.h"

class InnerBodyFrameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit InnerBodyFrameScene(const BodyFrame& bodyFrame, QObject *parent = nullptr);
};

#endif // INNERBODYFRAMESCENE_H
