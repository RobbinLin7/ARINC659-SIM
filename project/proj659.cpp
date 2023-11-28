#include "proj659.h"

Proj659::Proj659()
{

}

Proj659::~Proj659()
{
    if(save == true){
        //如果需要保存
    }
    delete projectTree;
}

Proj659::Proj659(QString name, QString description, QTreeWidgetItem* projectTree)
{
    this->name = name;
    this->description = description;
    this->projectTree = projectTree;
}

const QString &Proj659::getName() const
{
    return name;
}

void Proj659::setName(const QString &newName)
{
    name = newName;
}

const QString &Proj659::getDescription() const
{
    return description;
}

void Proj659::setDescription(const QString &newDescription)
{
    description = newDescription;
}

bool Proj659::getSave() const
{
    return save;
}

void Proj659::setSave(bool newSave)
{
    save = newSave;
}

uint Proj659::getMinUnusedId() const
{
//    二分法不行因为有可能某个bodyframeItem被删除了，用过的就不连续了
//    uint left = 0, right = maxFrameId;
//    while(left < right){
//        uint mid = (left + right) >> 1;
//        if(used[mid] == false) right = mid;
//        else left = mid + 1;
//    }
//    if(used[left] == false) return left;
//    else return maxFrameId + 1;
    uint i = 0;
    while(i <= maxFrameId && used[i]) ++i;
    return i;
}




bool Proj659::addBodyFrameItem(const BodyFrame& bodyFrameItem)
{
    //uint minUnusedId = getMinUnusedId();
    //bodyFrameItem.setBodyFrameItemID(minUnusedId);
    //std::shared_ptr<BodyFrameItem> item = std::shared_ptr<BodyFrameItem>(new BodyFrameItem());
    //item->setBodyFrameItemID(minUnusedId);
    //std::shared_ptr<BodyFrameGraphicsItem> item = std::shared_ptr<BodyFrameGraphicsItem>(new BodyFrameGraphicsItem(item));
    bodyFrameItems.insert(bodyFrameItem.getBodyFrameItemID(), bodyFrameItem);
    used[bodyFrameItem.getBodyFrameItemID()] = true;
    return true;
}

bool Proj659::deleteBodyFrameItem(uint id)
{
    used[id] = false;
    if(bodyFrameItems.contains(id) == false) return false;
    else{
        bodyFrameItems.remove(id);
        return true;
    }
}

BodyFrame& Proj659::getBodyFrameItem(uint id)
{
    return bodyFrameItems[id];
}

const BodyFrame &Proj659::getBodyFrameItem(uint id) const
{
    return bodyFrameItems[id];
}

Proj659::Status Proj659::getStatus() const
{
    return status;
}

void Proj659::setStatus(Status newStatus)
{
    status = newStatus;
}

const QMap<uint, BodyFrame> &Proj659::getBodyFrameItems() const
{
    return bodyFrameItems;
}

const QString& Proj659::getCommandFilePath()const{
    return commandFilePath;
}

void Proj659::setCommandFilePath(const QString &newCommandFilePath)
{
    this->commandFilePath = newCommandFilePath;
}

QTreeWidgetItem *Proj659::getProjectTree() const
{
    return projectTree;
}
