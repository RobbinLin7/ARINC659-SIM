#include "bodyFrameGraphicsItem.h"

#include <QImageReader>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QToolTip>
#include <QGraphicsScene>

BodyFrameGraphicsItem::~BodyFrameGraphicsItem()
{
    delete treeWidgetItem;
}

BodyFrameGraphicsItem::BodyFrameGraphicsItem(const BusGraphicsItem *Ax, const BusGraphicsItem *Ay, const BusGraphicsItem *Bx, const BusGraphicsItem *By, BodyFrame bodyFrameItem, QGraphicsItem *parent):
    QGraphicsItem(parent),
    img(":/resources/Image/bodyFrame.png"),
    toAx(Ax, this),
    toAy(Ay, this),
    toBx(Bx, this),
    toBy(By, this)
{
    this->bodyFrameItem = bodyFrameItem;
    setZValue(1);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    update();
}


QRectF BodyFrameGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, img.width() / 4, img.height() / 4);
}

QPainterPath BodyFrameGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void BodyFrameGraphicsItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *,
                          QWidget *)
{
    painter->drawImage(QRectF(0, 0, img.width() / 4, img.height() / 4), img);
    painter->save();
    painter->restore();
    computeLineToBus();
    //qDebug() << bodyFrameItem.getBodyFrameItemID() << pos();
}

void BodyFrameGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    qDebug() << "contextMenuEvent";
    QMenu menu;
    QAction *cfgBF = new QAction("配置机架");
    QAction *deleteBF = new QAction("删除机架");
    menu.addAction(cfgBF);
    menu.addAction(deleteBF);
    QPoint point(event->screenPos().x(), event->screenPos().y());
    QObject::connect(cfgBF, &QAction::triggered, this, [=](){
       emit(cfgBodyFrameItemSignal(this->bodyFrameItem.getBodyFrameItemID()));
    });
    connect(deleteBF, &QAction::triggered, this, [=](){
       emit(deleteBodyFrameItemSignal(this->bodyFrameItem.getBodyFrameItemID()));
    });
    menu.exec(point);
}

void BodyFrameGraphicsItem::computeLineToBus()
{
    qreal width = this->boundingRect().width() ;
    qreal height = this->boundingRect().height();
    toAx.setPos(width / 5, (2470 - this->y() + height) / 2);
    toAy.setPos(width * 2 / 5, (2490 - this->y() + height) / 2);
    toBx.setPos(width * 3 / 5, (2510 - this->y() + height) / 2);
    toBy.setPos(width * 4 / 5, (2530 - this->y() + height) / 2);
//    qDebug() << this->x() << toAx.x() << toAy.x() << toBx.x() << toBy.x();
    toAx.update();
    toAy.update();
    toBx.update();
    toBy.update();
}


void BodyFrameGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsItem::mousePressEvent(event);
}

void BodyFrameGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    if(scene()->collidingItems(this).isEmpty()){
//        previousPos = pos();
//        return QGraphicsItem::mouseMoveEvent(event);
//    }
//    else{
//        //qDebug() << "pos" << pos() <<  "eventPos" << event->scenePos();
//        setPos(previousPos);
//        qDebug() << "colliding";
//    }
    //return QGraphicsItem::mouseMoveEvent(event);
}


void BodyFrameGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    computeLineToBus();
    return QGraphicsItem::mouseReleaseEvent(event);
}

void BodyFrameGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QToolTip::showText(QCursor::pos(), QString("机架%1").arg(this->bodyFrameItem.getBodyFrameItemID()));
    return QGraphicsItem::hoverEnterEvent(event);
}

QVariant BodyFrameGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //防止移除到总线范围外
    if(change == QGraphicsItem::ItemPositionChange && this->scene()){
        QPointF newPos = value.toPointF();
        QRectF limitRec(500, 0, 4000 - this->boundingRect().height(), 2470 - this->boundingRect().width());
        if(limitRec.contains(newPos) == false){
            newPos.setX(qMin(limitRec.right(), qMax(newPos.x(), limitRec.left())));
            newPos.setY(qMin(limitRec.bottom(), qMax(newPos.y(), limitRec.top())));
            return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

QTreeWidgetItem *BodyFrameGraphicsItem::getTreeWidgetItem() const
{
    return treeWidgetItem;
}

void BodyFrameGraphicsItem::setTreeWidgetItem(QTreeWidgetItem *newTreeWidgetItem)
{
    treeWidgetItem = newTreeWidgetItem;
}
