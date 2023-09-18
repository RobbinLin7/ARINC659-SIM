#include "bodyframeitem.h"

BodyFrameItem::BodyFrameItem()
{
    std::cout << "constructor" << std::endl;
}

BodyFrameItem::BodyFrameItem(const BodyFrameItem &bodyFrameItem)
{
    std::cout << "copy constructor" << std::endl;
    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
    this->messageInterval = bodyFrameItem.messageInterval;
    this->modulesCnt = bodyFrameItem.modulesCnt;
    this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
    this->modules = new Module[this->modulesCnt];
    for(uint i = 0; i < this->modulesCnt; i++){
        this->modules[i] = bodyFrameItem.modules[i];
    }
    this->dataFrames = new DataFrame[this->dataFramesCnt];
    for(uint i = 0; i < this->dataFramesCnt; i++){
        this->dataFrames[i] = bodyFrameItem.dataFrames[i];
    }
}

BodyFrameItem::BodyFrameItem(BodyFrameItem &&bodyFrameItem)
{
    std::cout << "move constructor" <<  std::endl;
    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
    this->messageInterval = bodyFrameItem.messageInterval;
    this->modules = bodyFrameItem.modules;
    bodyFrameItem.modules = nullptr;
    this->modulesCnt = bodyFrameItem.modulesCnt;
    this->dataFrames = bodyFrameItem.dataFrames;
    bodyFrameItem.dataFrames = nullptr;
    this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
}

BodyFrameItem &BodyFrameItem::operator=(const BodyFrameItem &bodyFrameItem)
{
    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
    this->messageInterval = bodyFrameItem.messageInterval;
    this->modulesCnt = bodyFrameItem.modulesCnt;
    this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
    this->modules = new Module[this->modulesCnt];
    for(uint i = 0; i < this->modulesCnt; i++){
        this->modules[i] = bodyFrameItem.modules[i];
    }
    this->dataFrames = new DataFrame[this->dataFramesCnt];
    for(uint i = 0; i < this->dataFramesCnt; i++){
        this->dataFrames[i] = bodyFrameItem.dataFrames[i];
    }
    return *this;
}


BodyFrameItem &BodyFrameItem::operator=(BodyFrameItem&& bodyFrameItem){
    std::cout << "move assignment" << std::endl;
    if(this != &bodyFrameItem){
        std::cout << "move constructor" <<  std::endl;
        this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
        this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
        this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
        this->messageInterval = bodyFrameItem.messageInterval;
        this->modules = bodyFrameItem.modules;
        bodyFrameItem.modules = nullptr;
        this->modulesCnt = bodyFrameItem.modulesCnt;
        this->dataFrames = bodyFrameItem.dataFrames;
        bodyFrameItem.dataFrames = nullptr;
        this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
    }
    return *this;
}


BodyFrameItem::~BodyFrameItem()
{
    delete[] modules;           //方括号是必须的，因为我们当初分配的是一个数组
    delete[] dataFrames;
}


uint BodyFrameItem::getArbitrationStepDuration() const
{
    return arbitrationStepDuration;
}

void BodyFrameItem::setArbitrationStepDuration(uint newArbitrationStepDuration)
{
    arbitrationStepDuration = newArbitrationStepDuration;
}

uint BodyFrameItem::getTimeCalibrationFactor() const
{
    return timeCalibrationFactor;
}

void BodyFrameItem::setTimeCalibrationFactor(uint newTimeCalibrationFactor)
{
    timeCalibrationFactor = newTimeCalibrationFactor;
}

uint BodyFrameItem::getMajorVersionNumber() const
{
    return majorVersionNumber;
}

void BodyFrameItem::setMajorVersionNumber(uint newMajorVersionNumber)
{
    majorVersionNumber = newMajorVersionNumber;
}

uint BodyFrameItem::getSubVersionNumber() const
{
    return subVersionNumber;
}

void BodyFrameItem::setSubVersionNumber(uint newSubVersionNumber)
{
    subVersionNumber = newSubVersionNumber;
}

uint BodyFrameItem::getBodyFrameItemID() const
{
    return bodyFrameItemID;
}

void BodyFrameItem::setBodyFrameItemID(uint newBodyFrameItemID)
{
    bodyFrameItemID = newBodyFrameItemID;
}

uint BodyFrameItem::getMessageInterval() const
{
    return messageInterval;
}

void BodyFrameItem::setMessageInterval(uint newTimeInterval)
{
    messageInterval = newTimeInterval;
}
