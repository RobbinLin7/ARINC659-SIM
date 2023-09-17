#include "bodyframeitem.h"

BodyFrameItem::BodyFrameItem()
{

}

BodyFrameItem::BodyFrameItem(const BodyFrameItem &bodyFrameItem)
{
    std::cout << "copy constructor" << std::endl;
    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
    this->messageInterval = bodyFrameItem.messageInterval;
    this->modules = bodyFrameItem.modules;
    this->dataFrames = bodyFrameItem.dataFrames;
}

//BodyFrameItem::BodyFrameItem(BodyFrameItem &&bodyFrameItem)
//{
//    std::cout << "move constructor" <<  std::endl;
//    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
//    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
//    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
//    this->messageInterval = bodyFrameItem.messageInterval;
//    this->modules = bodyFrameItem.modules;
//    bodyFrameItem.modules = nullptr;
//    this->modulesCnt = bodyFrameItem.modulesCnt;
//    this->dataFrames = bodyFrameItem.dataFrames;
//    bodyFrameItem.dataFrames = nullptr;
//    this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
//}

//BodyFrameItem &BodyFrameItem::operator=(const BodyFrameItem &bodyFrameItem)
//{
//    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
//    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
//    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
//    this->messageInterval = bodyFrameItem.messageInterval;
//    this->modulesCnt = bodyFrameItem.modulesCnt;
//    this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
//    this->modules = new Module[this->modulesCnt];
//    for(uint i = 0; i < this->modulesCnt; i++){
//        this->modules[i] = bodyFrameItem.modules[i];
//    }
//    this->dataFrames = new DataFrame[this->dataFramesCnt];
//    for(uint i = 0; i < this->dataFramesCnt; i++){
//        this->dataFrames[i] = bodyFrameItem.dataFrames[i];
//    }
//    return *this;
//}


//BodyFrameItem &BodyFrameItem::operator=(BodyFrameItem&& bodyFrameItem){
//    std::cout << "move assignment" << std::endl;
//    if(this != &bodyFrameItem){
//        std::cout << "move constructor" <<  std::endl;
//        this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
//        this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
//        this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
//        this->messageInterval = bodyFrameItem.messageInterval;
//        this->modules = bodyFrameItem.modules;
//        bodyFrameItem.modules = nullptr;
//        this->modulesCnt = bodyFrameItem.modulesCnt;
//        this->dataFrames = bodyFrameItem.dataFrames;
//        bodyFrameItem.dataFrames = nullptr;
//        this->dataFramesCnt = bodyFrameItem.dataFramesCnt;
//    }
//    return *this;
//}


//BodyFrameItem::~BodyFrameItem()
//{
//    delete[] modules;           //方括号是必须的，因为我们当初分配的是一个数组
//    delete[] dataFrames;
//}


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


void BodyFrameItem::addModule(const Module &module)
{
    modules.insert(std::make_pair(module.getModuleNumber(), module));
}

void BodyFrameItem::modifyModule(const Module& module)
{
    if(modules.find(module.getModuleNumber()) != modules.end()){
        modules.at(module.getModuleNumber()) = module;
    }
}

void BodyFrameItem::deleteModule(uint moduleId)
{
    modules.erase(moduleId);
}

Module& BodyFrameItem::getModule(uint moduleId)
{
    return modules.find(moduleId)->second;
}

void BodyFrameItem::addDataFrame(const DataFrame &dataFrame)
{
    dataFrames[dataFrame.getFrameIdentification()] = dataFrame;
    dataFramesOrder.push_back(dataFrame.getFrameIdentification());
}

void BodyFrameItem::modifyDataFrame(const DataFrame& dataFrame)
{
    if(dataFrames.find(dataFrame.getFrameIdentification()) != dataFrames.end()){
        dataFrames.at(dataFrame.getFrameIdentification()) = dataFrame;
    }
}

void BodyFrameItem::deleteDataFrame(std::string dataFrameIdentification)
{
    dataFrames.erase(dataFrameIdentification);
}

DataFrame& BodyFrameItem::getDataFrame(std::string dataFrameIdentification)
{
    return dataFrames.at(dataFrameIdentification);
}

uint BodyFrameItem::getMinUnusedModuleId()
{
    uint i = 0;
    for(; i <= maxModuleNumber; i++){
        if(modules.find(i) != modules.end()){
            continue;
        }
        break;
    }
    return i;
}

const std::map<std::string, DataFrame> &BodyFrameItem::getDataFrames() const
{
    return dataFrames;
}

void BodyFrameItem::setDataFrames(const std::map<std::string, DataFrame> &newDataFrames)
{
    dataFrames = newDataFrames;
}

void BodyFrameItem::changeDataFramesOrder(int index1, int index2)
{
    std::swap(dataFramesOrder.at(index1), dataFramesOrder.at(index2));
}
