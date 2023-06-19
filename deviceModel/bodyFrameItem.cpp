#include "bodyFrameItem.h"

#include <QImageReader>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

BodyFrameItem::BodyFrameItem(QObject* parent):QObject(parent)
{
    setZValue(1);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    this->myHardwareModelNum = 0;
}

//BodyFrameItem::BodyFrameItem(BodyFrame *bf)
//{
//    this->myBodyFrame = bf;
//}

QRectF BodyFrameItem::boundingRect() const
{
    return imageRect;
}

QPainterPath BodyFrameItem::shape() const
{
    QPainterPath path;
    path.addRect(imageRect);
    return path;
}

void BodyFrameItem::paint(QPainter *painter,
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

void BodyFrameItem::setBodyFrameID(const uint &id)
{
    this->myBodyFrameID = id;
}

void BodyFrameItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton) return;
    QGraphicsItem::mousePressEvent(event);
    update();
}

void BodyFrameItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    qDebug() << "contextMenuEvent";
    QMenu menu;
    QAction *cfgBF = new QAction("配置机架");
    QAction *deleteBF = new QAction("删除机架");
    menu.addAction(cfgBF);
    menu.addAction(deleteBF);
    QPoint point(event->screenPos().x(), event->screenPos().y());
    QObject::connect(cfgBF, &QAction::triggered, this, [=](){
       emit(cfgBodyFrameItemSignal(this->myBodyFrameID));
    });
    menu.exec(point);

}

void BodyFrameItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        //stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void BodyFrameItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void BodyFrameItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    qDebug() << "mouse double click";
    update();
}

QSize BodyFrameItem::bodyFrameImageSize()
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
