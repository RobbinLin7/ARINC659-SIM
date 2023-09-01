#ifndef BODYFRAMEITEM_H
#define BODYFRAMEITEM_H
#include "data/module.h"
#include "data/dataframe.h"
#include <iostream>
#include <utility>
typedef unsigned int uint;
const uint maxFrameId = 15;
const uint minArbitrationStepDuration = 3, maxArbitrationStepDuration = 10;
const uint minTimeCalibrationFactor = 0, maxTimeCalibrationFactor = 63;
const uint minMessageInterval = 2, maxMessageInterval = 9;
const uint minModuleNumber = 0, maxModuleNumber = 31;
const uint minFramePeriod = 0, maxFramePeriod = 10000;
class BodyFrameItem
{
public:
    BodyFrameItem();

    BodyFrameItem(const BodyFrameItem&);    //拷贝构造函数

    BodyFrameItem(BodyFrameItem&&);         //移动构造函数

    BodyFrameItem &operator=(const BodyFrameItem&); //拷贝赋值运算符

    BodyFrameItem &operator=(BodyFrameItem&&);      //移动运算符

    ~BodyFrameItem();

    uint getArbitrationStepDuration() const;
    void setArbitrationStepDuration(uint newArbitrationStepDuration);

    uint getTimeCalibrationFactor() const;
    void setTimeCalibrationFactor(uint newTimeCalibrationFactor);

    uint getMajorVersionNumber() const;
    void setMajorVersionNumber(uint newMajorVersionNumber);

    uint getSubVersionNumber() const;
    void setSubVersionNumber(uint newSubVersionNumber);

    uint getBodyFrameItemID() const;
    void setBodyFrameItemID(uint newBodyFrameItemID);

    uint getMessageInterval() const;
    void setMessageInterval(uint newMessageInterval);

private:
     uint bodyFrameItemID;           //机架号
     uint arbitrationStepDuration;   //主/后备仲裁步进时长
     uint timeCalibrationFactor;     //时间标定因子
     uint messageInterval;           //消息间隔
     uint majorVersionNumber;        //主版本号
     uint subVersionNumber;          //次版本号
     Module* modules = nullptr;      //模块数组
     uint modulesCnt = 0;            //模块数组大小
     DataFrame* dataFrames = nullptr;//帧数组
     uint dataFramesCnt = 0;         //帧数组大小
};

#endif // BODYFRAMEITEM_H
