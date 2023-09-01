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
    :QObject(parent)
{
    this->bodyFrameItem = bodyFrameItem;
    setZValue(1);
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF BodyFrameGraphicsItem::boundingRect() const
{
    return imageRect;
}

QPainterPath BodyFrameGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(imageRect);
    return path;
}

void BodyFrameGraphicsItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *item,
                          QWidget *widget)
{

    //int width = bodyFrameImageSize().width();

    bodyFrameImageSize();


    painter->drawImage(imageRect, QImage(":/resources/Image/bodyFrame.png"));

    //在图片右上角标注出机架中的模块数

    QFont font("Times", 10);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font);
    painter->save();
    //painter->scale(0.1, 0.1);
    painter->drawText(imageRect.width() - 20, imageRect.height() - 20, QString(this->myHardwareModelNum));
    painter->restore();
}


void BodyFrameGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton) return;
    QGraphicsItem::mousePressEvent(event);
    update();
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

void BodyFrameGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        //stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void BodyFrameGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void BodyFrameGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    qDebug() << "mouse double click";
    update();
}

QSize BodyFrameGraphicsItem::bodyFrameImageSize()
{
    QString filename = ":/resources/Image/bodyFrame.png";
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    const QImage img = reader.read();

    imageRect.setX(0);
    imageRect.setY(0);
    imageRect.setWidth(img.size().width() / 3);
    imageRect.setHeight(img.size().height() / 3);
    return img.size();
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
