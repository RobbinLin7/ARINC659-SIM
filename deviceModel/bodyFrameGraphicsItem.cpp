#include "bodyFrameGraphicsItem.h"

#include <QImageReader>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

BodyFrameGraphicsItem::~BodyFrameGraphicsItem()
{
    delete treeWidgetItem;
}

BodyFrameGraphicsItem::BodyFrameGraphicsItem(BodyFrameItem bodyFrameItem, QObject* parent)
    :QObject(parent), img(":/resources/Image/bodyFrame.png")
{
    this->bodyFrameItem = bodyFrameItem;
    setZValue(1);
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF BodyFrameGraphicsItem::boundingRect() const
{
    return QRectF(this->x(), this->y() - 20, img.width() / 4, img.height() / 4 + 20);
}

QPainterPath BodyFrameGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void BodyFrameGraphicsItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *item,
                          QWidget *widget)
{
    static int i = 0;
    painter->drawImage(QRectF(250, 400, img.width() / 4, img.height() / 4), img);
    //在图片右上角标注出机架中的模块数

    QFont font("Times", 10);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font);

    //painter->scale(0.1, 0.1);

    painter->save();
    painter->restore();
    //update();
    //std::cout << i++ << " " << this->x() << " " << this->y() - 20 << std::endl;
    //painter->drawText(QPointF(0, -20) + pos(), QString("机架%1").arg(this->bodyFrameItem.getBodyFrameItemID()));
    //painter->save();
    //painter->restore();
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

void BodyFrameGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF pos = event->pos();

    // 保存当前的一些信息
    m_pos = pos;
    m_pressedPos = scenePos;
    m_startPos = this->pos();
    return QGraphicsItem::mousePressEvent(event);
}

void BodyFrameGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF pos = event->pos();

    // 计算偏移
    qreal xInterval = scenePos.x() - m_pressedPos.x();
    qreal yInterval = scenePos.y() - m_pressedPos.y();

    // 设置在场景中位置
    this->setPos(m_startPos + QPointF(xInterval, yInterval));
    this->update();
}

void BodyFrameGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout << "mouseReleaseEvent" << std::endl;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}



QTreeWidgetItem *BodyFrameGraphicsItem::getTreeWidgetItem() const
{
    return treeWidgetItem;
}

void BodyFrameGraphicsItem::setTreeWidgetItem(QTreeWidgetItem *newTreeWidgetItem)
{
    treeWidgetItem = newTreeWidgetItem;
    //connect(newTreeWidgetItem, &QTreeWidgetItem::)
}
