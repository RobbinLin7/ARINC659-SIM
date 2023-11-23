#ifndef INNERBODYFRAMESCENE_H
#define INNERBODYFRAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QMap>
#include "data/bodyframe.h"
#include "deviceModel/lrmgrphicsitem.h"
#include "deviceModel/busgraphicsitem.h"
#include <memory>

class InnerBodyFrameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit InnerBodyFrameScene(BodyFrame& bodyFrame, QObject *parent = nullptr);
    //explicit InnerBodyFrameScene();
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
signals:
    void exitBodyFrameSignal();
private:
    QMap<uint, std::shared_ptr<LRMGraphicsItem>> moduleGraphicItems;
    BodyFrame& bodyFrame;
private slots:
    void cfgModuleSlot(uint moduleId);
    void deleteModuleSlot(uint moduleId);
    //void addModuleSlot(uint moduleId);
};

#endif // INNERBODYFRAMESCENE_H
