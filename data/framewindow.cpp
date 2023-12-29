#include "framewindow.h"
#include <algorithm>
FrameWindow::FrameWindow()
{

}

FrameWindow::FrameWindow(int windowId):
    windowId(windowId)
{

}

bool FrameWindow::getFlag() const
{
    return flag;
}

void FrameWindow::setFlag(bool newFlag)
{
    flag = newFlag;
}

FrameWindow::WindowType FrameWindow::getWindowType() const
{
    return windowType;
}

void FrameWindow::setWindowType(WindowType newWindowType)
{
    windowType = newWindowType;
}

int FrameWindow::getWindowId() const
{
    return windowId;
}

void FrameWindow::setWindowId(int newWindowId)
{
    windowId = newWindowId;
}

int FrameWindow::getMainLRM() const
{
    return mainLRM;
}

void FrameWindow::setMainLRM(int newMainLRM)
{
    mainLRM = newMainLRM;
}

int FrameWindow::getDataCount() const
{
    return dataCount;
}

void FrameWindow::setDataCount(int newDataCount)
{
    dataCount = newDataCount;
}

int FrameWindow::getSendTimeScale() const
{
    return sendTimeScale;
}

void FrameWindow::setSendTimeScale(int newSendTimeScale)
{
    sendTimeScale = newSendTimeScale;
}

int FrameWindow::getIntNum1() const
{
    return intNum1;
}

void FrameWindow::setIntNum1(int newIntNum1)
{
    intNum1 = newIntNum1;
}

int FrameWindow::getIntNum2() const
{
    return intNum2;
}

void FrameWindow::setIntNum2(int newIntNum2)
{
    intNum2 = newIntNum2;
}

const std::string &FrameWindow::getSupportLRM1() const
{
    return supportLRM1;
}

void FrameWindow::setSupportLRM1(const std::string &newSupportLRM1)
{
    supportLRM1 = newSupportLRM1;
}

const std::string &FrameWindow::getSupportLRM2() const
{
    return supportLRM2;
}

void FrameWindow::setSupportLRM2(const std::string &newSupportLRM2)
{
    supportLRM2 = newSupportLRM2;
}

const std::string &FrameWindow::getSupportLRM3() const
{
    return supportLRM3;
}

void FrameWindow::setSupportLRM3(const std::string &newSupportLRM3)
{
    supportLRM3 = newSupportLRM3;
}

const std::string &FrameWindow::getSendAddr() const
{
    return sendAddr;
}

void FrameWindow::setStrSendAddr(const std::string &newStrSendAddr)
{
    sendAddr = newStrSendAddr;
}

const std::string &FrameWindow::getReceiveAddr() const
{
    return receiveAddr;
}

void FrameWindow::setStrReceiveAddr(const std::string &newStrReceveAddr)
{
    receiveAddr = newStrReceveAddr;
}

const std::string &FrameWindow::getSyncCode() const
{
    return syncCode;
}

void FrameWindow::setStrSyncCode(const std::string &newStrSynCode)
{
    syncCode = newStrSynCode;
}

const std::string &FrameWindow::getNewFrameID() const
{
    return newFrameID;
}

void FrameWindow::setStrNewFrameID(const std::string &newStrNewFrameID)
{
    newFrameID = newStrNewFrameID;
}

const std::list<int> &FrameWindow::getReceiveLRMList()const
{
    receiveLRMList.clear();
    std::copy(receiveLRMSet.begin(), receiveLRMSet.end(), std::back_inserter(receiveLRMList));
    return receiveLRMList;
}


void FrameWindow::removeAllOfReceiveLRMList()
{
    receiveLRMSet.clear();
}

void FrameWindow::setReceiveLRMList(const std::list<int> &newReceiveLRMList)
{

}


void FrameWindow::addReceiveLRM(const int LRM_id)
{
    receiveLRMSet.insert(LRM_id);
}

bool FrameWindow::isExistInReceiveLRMList(const int LRM_id)
{
    if(std::find(receiveLRMList.begin(),receiveLRMList.end(),LRM_id)!=receiveLRMList.end())
        return true;
    else return false;

}

const std::string &FrameWindow::getDataSourceFile() const
{
    return dataSourceFile;
}

void FrameWindow::setDataSourceFile(const std::string &newDataSourceFile)
{
    dataSourceFile = newDataSourceFile;
}

bool FrameWindow::getFinished() const
{
    return finished;
}

void FrameWindow::setFinished(bool newFinished)const
{
    finished = newFinished;
}

uint32_t FrameWindow::getNumOfTimeSlot() const
{
    return numOfTimeSlot;
}

void FrameWindow::setNumOfTimeSlot(const uint32_t &value)
{
    numOfTimeSlot = value;
}

bool operator==(const FrameWindow& frameWindow1, const FrameWindow& frameWindow2){
    return (frameWindow1.getWindowId() == frameWindow2.getWindowId()) &&
           (frameWindow1.getWindowType() == frameWindow2.getWindowType()) &&
           (frameWindow1.getDataCount() == frameWindow2.getDataCount()) &&
           (frameWindow1.getDataSourceFile() == frameWindow2.getDataSourceFile()) &&
           (frameWindow1.getFinished() == frameWindow2.getFinished()) &&
           (frameWindow1.getFlag() == frameWindow2.getFlag()) &&
           (frameWindow1.getIntNum1() == frameWindow2.getIntNum1()) &&
           (frameWindow1.getIntNum2() == frameWindow2.getIntNum2()) &&
           (frameWindow1.getMainLRM() == frameWindow2.getMainLRM()) &&
           (frameWindow1.getNewFrameID() == frameWindow2.getNewFrameID()) &&
           (frameWindow1.getNumOfTimeSlot() == frameWindow2.getNumOfTimeSlot()) &&
           (frameWindow1.getReceiveAddr() == frameWindow2.getReceiveAddr()) &&
           (frameWindow1.getReceiveLRMList() == frameWindow2.getReceiveLRMList()) &&
           (frameWindow1.getSendAddr() == frameWindow2.getSendAddr()) &&
           (frameWindow1.getSendTimeScale() == frameWindow2.getSendTimeScale()) &&
           (frameWindow1.getSupportLRM1() == frameWindow2.getSupportLRM1()) &&
           (frameWindow1.getSupportLRM2() == frameWindow2.getSupportLRM2()) &&
           (frameWindow1.getSupportLRM3() == frameWindow2.getSupportLRM3()) &&
           (frameWindow1.getSyncCode() == frameWindow2.getSyncCode());
}
