#include "dataframe.h"

DataFrame::DataFrame()
{

}

DataFrame &DataFrame::operator=(const DataFrame &dataFrame)
{
    this->framePeriod = dataFrame.framePeriod;
    this->totalWindow = dataFrame.totalWindow;
    //this->frameIdentification = dataFrame.frameIdentification;
    this->idleWaitTime = dataFrame.idleWaitTime;
    this->timeAllocationType = dataFrame.timeAllocationType;
    return *this;
}

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
