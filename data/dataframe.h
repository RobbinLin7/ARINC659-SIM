#ifndef DATAFRAME_H
#define DATAFRAME_H
#include<string>
typedef unsigned int uint;
const uint minFramePeriod = 0, maxFramePeriod = 10000;
class DataFrame
{
public:
    enum TimeAllocationType{equalAlloc, downConcentrationAlloc};
    DataFrame();
    DataFrame& operator=(const DataFrame&);

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

private:
    uint idleWaitTime;                      //空闲等待位时
    uint framePeriod;                       //帧周期
    uint totalWindow;                       //窗口总数
    std::string frameIdentification;        //帧标识
    TimeAllocationType timeAllocationType;  //时间分配（平均分配，向下集中）
};

#endif // DATAFRAME_H
