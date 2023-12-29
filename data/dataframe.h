#ifndef DATAFRAME_H
#define DATAFRAME_H
#include<string>
#include<vector>
#include"data/framewindow.h"
typedef unsigned int uint;
const uint minFramePeriod = 0, maxFramePeriod = 10000;
const uint maxWindowCnt = 128;
class DataFrame
{
public:
    enum TimeAllocationType{equalAlloc, downConcentrationAlloc};
    enum DataFrameType{Parent, Child};

    DataFrame();
    //DataFrame& operator=(const DataFrame&);

    uint getFramePeriod() const;
    void setFramePeriod(uint newFramePeriod);

    uint getTotalWindow() const;
    void setTotalWindow(uint newTotalWindow);

    const std::string &getFrameIdentification() const;
    void setFrameIdentification(const std::string &newFrameIdentification);

    uint getIdleWaitTime() const;
    void setIdleWaitTime(uint newIdleWaitTime);

    TimeAllocationType getTimeAllocationType() const;
    void setTimeAllocationType(TimeAllocationType newTimeAllocationType);

    DataFrameType getDataFrameType() const;
    void setDataFrameType(DataFrameType newDataFrameType);

    const std::vector<FrameWindow> &getFrameWindows() const;

    std::vector<FrameWindow> &getFrameWindows();

    void insertFrameWindow(const FrameWindow&);

    void deleteFrameWindowAtIndex(const uint index);

    void moveUpFrameWindowAtIndex(const uint index);

    void moveDownFrameWindowAtIndex(const uint index);

    bool getRetWithGap() const;

    uint32_t getTotalTimeSlot() const;
    void setTotalTimeSlot(const uint32_t &value);

private:
    bool windowIdUsed[maxWindowCnt];
    uint idleWaitTime;                      //空闲等待位时
    uint framePeriod;                       //帧周期
    uint totalWindow;                       //窗口总数
    uint32_t totalTimeSlot = 0;
    std::string frameIdentification;        //帧标识
    TimeAllocationType timeAllocationType;  //时间分配（平均分配，向下集中）
    DataFrameType dataFrameType;            //区分是否为子帧
    std::vector<FrameWindow> frameWindows;    //窗口列表
    bool retWithGap = false;                  //子序列返回时是否发送固有空闲，即利用RET，还是RETI返回
};

bool operator==(const DataFrame& dataFrame1, const DataFrame& dataFrame2);

#endif // DATAFRAME_H
