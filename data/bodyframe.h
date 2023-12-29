#ifndef BODYFRAMEITEM_H
#define BODYFRAMEITEM_H
#include "data/module.h"
#include "data/dataframe.h"
#include "data/dataframes.h"
#include <iostream>
#include <utility>
#include <vector>
#include <map>
typedef unsigned int uint;
const uint minFrameId = 0, maxFrameId = 15;
const uint minArbitrationStepDuration = 3, maxArbitrationStepDuration = 10;
const uint minTimeCalibrationFactor = 0, maxTimeCalibrationFactor = 63;
const uint minMessageInterval = 2, maxMessageInterval = 9;
class BodyFrame
{
public:
    BodyFrame();

//    BodyFrameItem(const BodyFrameItem&);    //拷贝构造函数

//    BodyFrameItem(BodyFrameItem&&);         //移动构造函数

//    BodyFrameItem &operator=(const BodyFrameItem&); //拷贝赋值运算符

//    BodyFrameItem &operator=(BodyFrameItem&&);      //移动运算符

    //~BodyFrameItem();

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


    //模块数组和帧数组的增、删、改、查
    void addModule(const Module&);
    void modifyModule(const Module&);
    void deleteModule(uint);
    Module& getModule(uint);
    void addDataFrame(const DataFrame&);
    void modifyDataFrame(const DataFrame&);
    void deleteDataFrame(std::string);
    DataFrame& getDataFrame(std::string);
    uint getMinUnusedModuleId();

    const DataFrames &getDataFrames() const;
    void setDataFrames(const DataFrames &newDataFrames);

    void changeDataFramesOrder(int index1, int index2);
    //const std::vector<std::string>& getDataFramesOrder() const;

    const std::map<uint, Module> &getModules() const;
    void setModules(const std::map<uint, Module> &newModules);

private:
     mutable uint bodyFrameItemID;                  //机架号
     uint arbitrationStepDuration;                  //主/后备仲裁步进时长
     uint timeCalibrationFactor;                    //时间标定因子
     uint messageInterval;                          //消息间隔
     uint majorVersionNumber;                       //主版本号
     uint subVersionNumber;                         //次版本号
     std::map<uint, Module> modules;                //模块数组
//     std::map<std::string, DataFrame> dataFrames;   //帧数组
//     std::vector<std::string> dataFramesOrder;      //帧数组顺序
     DataFrames dataFrames;
};

bool operator==(const BodyFrame& bodyFrame1, const BodyFrame& bodyFrame2);

#endif // BODYFRAMEITEM_H
