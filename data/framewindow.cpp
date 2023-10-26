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
