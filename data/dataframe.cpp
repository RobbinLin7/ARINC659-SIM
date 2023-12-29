#include "dataframe.h"
#include<cstring>

DataFrame::DataFrame()
{
    memset(windowIdUsed, 0, sizeof (windowIdUsed));
}

//DataFrame &DataFrame::operator=(const DataFrame &dataFrame)
//{
//    this->framePeriod = dataFrame.framePeriod;
//    this->totalWindow = dataFrame.totalWindow;
//    this->frameIdentification = dataFrame.frameIdentification;
//    this->idleWaitTime = dataFrame.idleWaitTime;
//    this->timeAllocationType = dataFrame.timeAllocationType;
//    this->frameWindows = dataFrame.frameWindows;
//    return *this;
//}

uint DataFrame::getFramePeriod() const
{
    return framePeriod;
}

void DataFrame::setFramePeriod(uint newFramePeriod)
{
    framePeriod = newFramePeriod;
}

uint DataFrame::getTotalWindow() const
{
    return totalWindow;
}

void DataFrame::setTotalWindow(uint newTotalWindow)
{
    totalWindow = newTotalWindow;
}

const std::string &DataFrame::getFrameIdentification() const
{
    return frameIdentification;
}

void DataFrame::setFrameIdentification(const std::string &newFrameIdentification)
{
    frameIdentification = newFrameIdentification;
}

uint DataFrame::getIdleWaitTime() const
{
    return idleWaitTime;
}

void DataFrame::setIdleWaitTime(uint newIdleWaitTime)
{
    idleWaitTime = newIdleWaitTime;
}

DataFrame::TimeAllocationType DataFrame::getTimeAllocationType() const
{
    return timeAllocationType;
}

void DataFrame::setTimeAllocationType(TimeAllocationType newTimeAllocationType)
{
    timeAllocationType = newTimeAllocationType;
}

DataFrame::DataFrameType DataFrame::getDataFrameType() const
{
    return dataFrameType;
}

void DataFrame::setDataFrameType(DataFrameType newDataFrameType)
{
    dataFrameType = newDataFrameType;
}

const std::vector<FrameWindow> &DataFrame::getFrameWindows() const
{
    return frameWindows;
}

std::vector<FrameWindow> &DataFrame::getFrameWindows()
{
    return frameWindows;
}

void DataFrame::insertFrameWindow(const FrameWindow &frameWindow)
{
    frameWindows.push_back(frameWindow);
}

void DataFrame::deleteFrameWindowAtIndex(const uint index)
{
    frameWindows.erase(frameWindows.begin() + index);
}

void DataFrame::moveUpFrameWindowAtIndex(const uint index)
{
    std::swap(frameWindows[index], frameWindows[index - 1]);
}

void DataFrame::moveDownFrameWindowAtIndex(const uint index)
{
    std::swap(frameWindows[index], frameWindows[index + 1]);
}

bool DataFrame::getRetWithGap() const
{
    return retWithGap;
}

uint32_t DataFrame::getTotalTimeSlot() const
{
    return totalTimeSlot;
}

void DataFrame::setTotalTimeSlot(const uint32_t &value)
{
    totalTimeSlot = value;
}

bool operator==(const DataFrame& dataFrame1, const DataFrame& dataFrame2){
    bool flag = (dataFrame1.getFrameWindows() == dataFrame2.getFrameWindows());
    flag = flag && (dataFrame1.getFrameIdentification() == dataFrame2.getFrameIdentification());
    flag = flag && (dataFrame1.getFramePeriod() == dataFrame2.getFramePeriod());
    flag = flag && (dataFrame1.getIdleWaitTime() == dataFrame2.getIdleWaitTime());
    flag = flag && ((bool)dataFrame1.getRetWithGap() == (bool)dataFrame2.getRetWithGap());
    flag = flag && (dataFrame1.getTimeAllocationType() == dataFrame2.getTimeAllocationType());
    flag = flag && (dataFrame1.getTotalTimeSlot() == dataFrame2.getTotalTimeSlot());
    flag = flag && (dataFrame1.getTotalWindow() == dataFrame2.getTotalWindow());
    flag = flag && (dataFrame1.getDataFrameType() == dataFrame2.getDataFrameType());
    return (flag = (dataFrame1.getDataFrameType() == dataFrame2.getDataFrameType()) &&
           (dataFrame1.getFrameIdentification() == dataFrame2.getFrameIdentification()) &&
           (dataFrame1.getFramePeriod() == dataFrame2.getFramePeriod()) &&
           (dataFrame1.getIdleWaitTime() == dataFrame2.getIdleWaitTime()) &&
           (dataFrame1.getRetWithGap() == dataFrame2.getRetWithGap()) &&
           (dataFrame1.getTimeAllocationType() == dataFrame2.getTimeAllocationType()) &&
           (dataFrame1.getTotalTimeSlot() == dataFrame2.getTotalTimeSlot()) &&
           (dataFrame1.getTotalWindow() == dataFrame2.getTotalWindow()) &&
           (dataFrame1.getFrameWindows() == dataFrame2.getFrameWindows()));
}
