#include "bodyframe.h"

BodyFrame::BodyFrame()
{
    std::cout << "constructor" << std::endl;
}

//BodyFrameItem::BodyFrameItem(const BodyFrameItem &bodyFrameItem)
//{
//    std::cout << "copy constructor" << std::endl;
//    this->bodyFrameItemID = bodyFrameItem.bodyFrameItemID;
//    this->arbitrationStepDuration = bodyFrameItem.arbitrationStepDuration;
//    this->timeCalibrationFactor = bodyFrameItem.timeCalibrationFactor;
//    this->messageInterval = bodyFrameItem.messageInterval;
//    this->modules = bodyFrameItem.modules;
//    this->dataFrames = bodyFrameItem.dataFrames;
//    this->majorVersionNumber = bodyFrameItem.majorVersionNumber;
//    this->subVersionNumber = bodyFrameItem.subVersionNumber;
//}

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


uint BodyFrame::getArbitrationStepDuration() const
{
    return arbitrationStepDuration;
}

void BodyFrame::setArbitrationStepDuration(uint newArbitrationStepDuration)
{
    arbitrationStepDuration = newArbitrationStepDuration;
}

uint BodyFrame::getTimeCalibrationFactor() const
{
    return timeCalibrationFactor;
}

void BodyFrame::setTimeCalibrationFactor(uint newTimeCalibrationFactor)
{
    timeCalibrationFactor = newTimeCalibrationFactor;
}

uint BodyFrame::getMajorVersionNumber() const
{
    return majorVersionNumber;
}

void BodyFrame::setMajorVersionNumber(uint newMajorVersionNumber)
{
    majorVersionNumber = newMajorVersionNumber;
}

uint BodyFrame::getSubVersionNumber() const
{
    return subVersionNumber;
}

void BodyFrame::setSubVersionNumber(uint newSubVersionNumber)
{
    subVersionNumber = newSubVersionNumber;
}

uint BodyFrame::getBodyFrameItemID() const
{
    return bodyFrameItemID;
}

void BodyFrame::setBodyFrameItemID(uint newBodyFrameItemID)
{
    bodyFrameItemID = newBodyFrameItemID;
}

uint BodyFrame::getMessageInterval() const
{
    return messageInterval;
}

void BodyFrame::setMessageInterval(uint newTimeInterval)
{
    messageInterval = newTimeInterval;
}

void BodyFrame::addModule(const Module &module)
{
    modules.insert(std::make_pair(module.getModuleNumber(), module));
}

void BodyFrame::modifyModule(const Module& module)
{
    if(modules.find(module.getModuleNumber()) != modules.end()){
        modules.at(module.getModuleNumber()) = module;
    }
}

void BodyFrame::deleteModule(uint moduleId)
{
    modules.erase(moduleId);
}

Module& BodyFrame::getModule(uint moduleId)
{
    return modules.find(moduleId)->second;
}

void BodyFrame::addDataFrame(const DataFrame &dataFrame)
{
    dataFrames.push(dataFrame);
    //dataFrames[dataFrame.getFrameIdentification()] = dataFrame;
    //dataFramesOrder.push_back(dataFrame.getFrameIdentification());
}

void BodyFrame::modifyDataFrame(const DataFrame& dataFrame)
{
    DataFrames::Iterator itr = dataFrames.find(dataFrame);
    if(itr != dataFrames.end()){
        *itr = dataFrame;
    }
}

void BodyFrame::deleteDataFrame(std::string dataFrameIdentification)
{
    dataFrames.erase(dataFrameIdentification);
}

DataFrame& BodyFrame::getDataFrame(std::string dataFrameIdentification)
{
    return dataFrames.at(dataFrameIdentification);
}

uint BodyFrame::getMinUnusedModuleId()
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

const DataFrames& BodyFrame::getDataFrames() const
{
    return dataFrames;
}

void BodyFrame::setDataFrames(const DataFrames& newDataFrames)
{
    dataFrames = newDataFrames;
}

void BodyFrame::changeDataFramesOrder(int index1, int index2)
{
    dataFrames.swap(index1, index2);
}

const std::map<uint, Module> &BodyFrame::getModules() const
{
    return modules;
}

void BodyFrame::setModules(const std::map<uint, Module> &newModules)
{
    modules = newModules;
}
