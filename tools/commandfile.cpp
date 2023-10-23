#include "commandfile.h"
#include <fstream>
#include "tools/preprocess.h"
using std::to_string;
using std::string;

std::string trim(std::string s){
    if (s.empty())
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

std::string trimStart(std::string s){
    if(s.empty()){
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    return s;
}
bool CommandFile::createCommandFile(const Proj659 &proj)
{
    std::ofstream commandFile(proj.getName().toStdString() + ".txt");
    for(auto bodyFrame : proj.getBodyFrameItems()){
        createBodyFrameInfo(commandFile, bodyFrame);
        commandFile << "     ESECTION ;" << std::endl;
    }
    commandFile << "      END;" << std::endl;
    return true;
}
void VarInit(COMPILE_STATUS& c_status);
bool CommandFile::compileCommandFile(const Proj659 &proj)
{
    COMPILE_STATUS com_status;
    std::list<LABEL_TABLE> label_list;
    uint nTmp;
    /* 变量初始化 */
    VarInit(com_status);
    /* 源文件预处理 */
//    PreProcess preProcess(m_strDir, m_strFlieName);
//    preProcess.m_lstSourceCommand = m_lstSourceCommand;
//    if (preProcess.ProcessCommand(ref com_status) != 0)
//    {
//        return 1;
//    }
}


CommandFile::CommandFile()
{

}

bool CommandFile::createBodyFrameInfo(std::ostream &os, const BodyFrame &bodyFrame)
{
    //模块宏定义
    for(auto modulePair : bodyFrame.getModules()){
        auto module = modulePair.second;
    }

    //输出机架号
    os << "    SECTION  " << bodyFrame.getBodyFrameItemID() << std::endl;

    //输出时间标定因子
    os << "        TIMESCALE  " << bodyFrame.getTimeCalibrationFactor() << std::endl;

    //输出消息间隔
    os << "        GAP  " << bodyFrame.getMessageInterval() << std::endl;

    //输出主后备仲裁步进时
    os << "        DELTA  " << bodyFrame.getArbitrationStepDuration() << std::endl;

    //输出版本号
    os << "        VER  " << bodyFrame.getMajorVersionNumber() << " " << bodyFrame.getSubVersionNumber()
       << " " << std::to_string(bodyFrame.getBodyFrameItemID()) + " ;" << std::endl;

    //输出模块初始化等待时间
    if(bodyFrame.getModules().empty() == true){
        os << "        INITWAITTIME 0XFFFFFFFF ; " << std::endl;
        return true;
    }
    os << "       INITWAITTIME ";
    int n_tmp = bodyFrame.getModules().cbegin()->second.getModuleNumber();
    for(int i = 0; i < n_tmp; i++){
        os << "   0XFFFFFFFF ";
    }
    os << " " << bodyFrame.getModules().cbegin()->second.getInitialWaitTime();

    for(auto modulePair : bodyFrame.getModules()){
        auto module = modulePair.second;
        os << "   0XFFFFFFFF ";
    }
    os << " " << bodyFrame.getModules().cbegin()->second.getInitialWaitTime();

    for(auto pre = bodyFrame.getModules().cbegin(),
        now = ++bodyFrame.getModules().cbegin(); now != bodyFrame.getModules().cend(); pre++, now++){
        n_tmp = now->second.getModuleNumber() - pre->second.getModuleNumber();
        for(int j = 0; j < n_tmp - 1; j++){
            os << " 0XFFFFFFFF ";
        }
        os << " " << now->second.getInitialWaitTime();
    }
    os << ";" << std::endl;
}

bool CommandFile::createCommand(std::ofstream &os, const BodyFrame& bodyFrame)
{
    std::string str_tmp = "";
    int i, j;
    int nAverageFree = 0; //每个窗口的平均释放时间
    int nTotalFree = 0;      //指定帧的总的释放时间
    int nTmp;
    int nWindowType;
    int nFreeTime;
    int nTotalFreeTime;
    int nGap, nDelta;
    nGap = bodyFrame.getMessageInterval();
    nDelta = bodyFrame.getArbitrationStepDuration();

    auto dataFrames = bodyFrame.getDataFrames();
    auto dataFramesOrder = bodyFrame.getDataFramesOrder();
    if(dataFramesOrder.size() == 0){
        return false;
    }
    for(auto dataFrameId : dataFramesOrder){
        auto dataFrame = dataFrames[dataFrameId];
        nFreeTime = dataFrame.getIdleWaitTime();
        if(dataFrame.getDataFrameType() == DataFrame::Child){
            //子帧
            os << " " << dataFrame.getFrameIdentification() + " SUB SSYNC ; ";
        }
        else{
            //父帧
            os << " " << dataFrame.getFrameIdentification() + "   SSYNC ; ";
        }
        int nResult = 0;

        int nFrameCyl = dataFrame.getFramePeriod() * 30000; //时钟频率为30Mhz时,一个周期内的总位时长.

        int nTotalWindowTime = getTotalCyl(bodyFrame, dataFrame);

        if(nFrameCyl < nTotalFreeTime){
            return false;   //若窗口时间综合大于帧周期，则返回错误
        }
        nTotalFreeTime = nFrameCyl - nTotalWindowTime;  //总的施放时间为当前帧的时间减去窗口总时间

        if(dataFrame.getTimeAllocationType() == DataFrame::equalAlloc){
            createFrameAverage(os, dataFrame, bodyFrame, nTotalFreeTime);
        }
        else if(dataFrame.getTimeAllocationType() == DataFrame::downConcentrationAlloc){
            createFrameFreeTimeDown(os, dataFrame, bodyFrame, nTotalFreeTime);
        }
    }
    return true;
}

int CommandFile::getTotalCyl(const BodyFrame &bodyFrame, const DataFrame &dataFrame)
{
    int nTotal = 0;
    FrameWindow::WindowType nWindowType;
    int nTmp;
    int nMaxGap = 9;
    int nMaxDelta = 10;
    int nGap = bodyFrame.getMessageInterval();
    int nDelta = bodyFrame.getArbitrationStepDuration();
    auto frames = bodyFrame.getDataFrames();
    auto dataFramesOrder = bodyFrame.getDataFramesOrder();
    for(auto& window : dataFrame.getFrameWindows()){
        nWindowType = window.getWindowType();
        switch (nWindowType) {
        case FrameWindow::DATA_SEND:
            if(window.getSupportLRM1() == "" && window.getSupportLRM2() == "" && window.getSupportLRM3() == ""){
                nTmp = 16 * window.getDataCount() + nGap;
            }
            else nTmp = 16 * window.getDataCount() + nGap + 3 * nDelta;

            //主、后备发送
            nTotal += nTmp;
            break;
        case FrameWindow::VERSION_SEND:
            //发送接收数据
            nTmp = 16 * 2 + nGap;
            nTotal += nTmp;
            break;
        case FrameWindow::LONG_SYNC:
            //发送长同步
            nTmp = 136 + 3 * 10 + 2 * 9;
            nTotal += nTmp;
            break;
        case FrameWindow::FRAME_SWITCH:
            //发送帧切换
            nTmp = 136 + 3 * 10 + 2 * 9;
            nTotal += nTmp;
            break;
        case FrameWindow::CALL_SUBFRRAME:
            //调用子帧
            if(window.getFlag()){
                nTotal += 5 + nGap; //发送固有空闲
            }
            for(int j = 0; j < frames.size(); j++){
                if(window.getNewFrameID() == frames[dataFramesOrder[j]].getFrameIdentification()){
                    nTotal += frames[dataFramesOrder[j]].getFramePeriod() * 30000;
                }
            }
            break;
        case FrameWindow::INT_SEND:
            //发送中断
            //中断占用时间尚未确定
            break;
        case FrameWindow::FRAME_JUMP:
            //跳转指令
            if(window.getFlag()){
                nTotal += 16 + nGap;
            }
            goto EndCommand;
        case FrameWindow::FREE:
            nTotal += window.getDataCount();
            break;
        case FrameWindow::SHORT_SYNC:
            nTotal += 5 + nGap;
            break;
        default:
            break;
        }
    }

    EndCommand:
    //从子序列返回并发送固有空闲
    if(dataFrame.getRetWithGap() && dataFrame.getDataFrameType() == DataFrame::Child){
        nTotal += 16 + nGap;    //发送固有空闲
    }

    return nTotal;
}

bool CommandFile::createFrameAverage(std::ostream &os, const DataFrame& dataFrame, const BodyFrame& bodyFrame, int nTotalFreeTime)
{
    std::string str_tmp;
    int i, j;
    int nAverageFree = 0;   //每个窗口的平均释放时间
    int nTotalFree = 0;     //指定帧的总的释放时间
    int nTmp;
    FrameWindow::WindowType nWindowType;
    int nSendWindowCount;
    int nDataSendFreeTime;  //发送数据窗口可释放的空闲等待时间
    int nSendIntTime;
    std::string str_supportLRM;
    int nGap, nDelta;
    nGap = bodyFrame.getMessageInterval();
    nDelta = bodyFrame.getArbitrationStepDuration();
    int nDefaultFreeTime;


    nDefaultFreeTime = dataFrame.getIdleWaitTime()+5+nGap;//空闲等待时间

    nTotalFreeTime = nTotalFreeTime - 5 - nGap;

    if (nDefaultFreeTime > 0)
    {

        if (nDefaultFreeTime * dataFrame.getFrameWindows().size() > nTotalFreeTime)
        {
            nDefaultFreeTime = nTotalFreeTime / dataFrame.getFrameWindows().size();
        }

        nDataSendFreeTime = nTotalFreeTime - dataFrame.getFrameWindows().size() * (nDefaultFreeTime);

    }
    else nDataSendFreeTime = nTotalFreeTime;


    nSendWindowCount = 0;
    for(auto itr = dataFrame.getFrameWindows().cbegin(); itr != dataFrame.getFrameWindows().cend(); ++itr){
        if(itr->getWindowType() == FrameWindow::DATA_SEND){
            nSendWindowCount++;
        }
    }
    nAverageFree = 0;
    if (nSendWindowCount > 0 && nDataSendFreeTime > 0)
    {
        nAverageFree = nDataSendFreeTime / nSendWindowCount;
    }
    auto itr = dataFrame.getFrameWindows().cbegin();
    for (j = 0; j < dataFrame.getFrameWindows().size(); j++, ++itr)
    {
        os << " " << std::endl;
        if (j < dataFrame.getFrameWindows().size() - 1)
        {
            writeFreeTime(os, nDefaultFreeTime, nGap);
        }
        else
        {
            nTmp = nTotalFreeTime - nDefaultFreeTime * (dataFrame.getFrameWindows().size() - 1) - nAverageFree * nSendWindowCount;
            writeFreeTime(os, nTmp, nGap);
        }
        nWindowType = itr->getWindowType();
        //nWindowType = itr->m_WindowType;
        switch (nWindowType)
        {
            case FrameWindow::DATA_SEND: {//发送接收数据
                if (itr->getIntNum1() > 15)
                {
                    str_tmp = " ";
                    str_supportLRM = trim(itr->getSupportLRM1());
                    if (str_tmp != "")
                    {
                        str_tmp += convertModuleNumToName(bodyFrame, str_supportLRM);
                       // str_tmp+= convertModuleNumToName(bodyFrame, str_supportLRM);
                    }
                    str_supportLRM = trim(itr->getSupportLRM2());
                    //str_supportLRM = itr->m_strSupportLRM2.Trim();
                    if (str_tmp != "")
                    {
                        str_tmp +=  convertModuleNumToName(bodyFrame, str_supportLRM);

                    }
                    str_supportLRM = trim(itr->getSupportLRM3());
                    //str_supportLRM = itr->m_strSupportLRM3.Trim();
                    if (str_tmp != "")
                    {
                        str_tmp += convertModuleNumToName(bodyFrame, str_supportLRM);
                    }


                    str_tmp = "      INT  " + std::to_string(itr->getIntNum1()) + ","
                        + convertModuleNumToName(bodyFrame, itr->getMainLRM())
                        + str_tmp + " ; ";

                    os << str_tmp << std::endl;
                }

                double temp = 1.0 * itr->getSendTimeScale() / 10.0;
//                double temp = Convert.ToDouble(itr->m_nSendTimeScale);

//                temp = temp / 10.0;

//                nSendIntTime = Convert.ToInt32(Convert.ToDouble(nAverageFree)* temp);

                nSendIntTime = nAverageFree * temp;

                nTmp = nSendIntTime;
                writeFreeTime(os, nTmp, nGap);//写入空闲等待

                str_tmp = "      BOW  " + std::to_string(itr->getDataCount()) + " ;";
                os << str_tmp << std::endl;
                //os << str_tmp << std::endl;

                str_tmp = "      TX   "
                    + convertModuleNumToName(bodyFrame, itr->getMainLRM())
                    + " "+ itr->getSendAddr() + " ;";

                os << str_tmp << std::endl;
                //os << str_tmp << std::endl;

                //后备模块1
                str_tmp = itr->getSupportLRM1();
                str_tmp = trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                +convertModuleNumToName(bodyFrame, itr->getSupportLRM1()) + " "
                                + itr->getSendAddr() + " ;";

                    os << str_tmp << std::endl;
                }

                //后备模块2
                str_tmp = itr->getSupportLRM2();
                str_tmp = trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                +convertModuleNumToName(bodyFrame, itr->getSupportLRM2()) + " "
                                + itr->getSendAddr() + " ;";

                    os << str_tmp << std::endl;

                }

                //后备模块3
                str_tmp = itr->getSupportLRM3();
                str_tmp = trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                          +convertModuleNumToName(bodyFrame, itr->getSupportLRM3()) + " "
                          + itr->getSendAddr() + " ;";

                    os << str_tmp << std::endl;

                }

                //接收模块
                for(auto x : itr->getReceiveLRMList()){
                    str_tmp = "      RX   "

                        +convertModuleNumToName(bodyFrame, x) + " "
                        + itr->getReceiveAddr() + " ;";

                    os << str_tmp << std::endl;
                }
//                for (int k = 0; k < itr->m_ReceiveLRMList.si; k++)
//                {

//                    str_tmp = "      RX   "

//                        +convertModuleNumToName(bodyFrame, itr->m_ReceiveLRMList[k]) + " "
//                        + itr->m_strReceveAddr + " ;";

//                    os << str_tmp << std::endl;


//                }

                nTmp = nAverageFree - nSendIntTime;

                writeFreeTime(os, nTmp, nGap);//写入空闲等待

                //#region 使用中断接收数据


                if (itr->getIntNum2()>15)
                {

                    str_tmp = " ";

                    for(auto x : itr->getReceiveLRMList()){
                        str_tmp += " " + convertModuleNumToName(bodyFrame, x);
                    }

//                    for (int k = 0; k < itr->m_ReceiveLRMList.Count; k++)
//                    {

//                        str_tmp += " " + convertModuleNumToName(bodyFrame, itr->m_ReceiveLRMList[k]);

//                    }

                    str_tmp = "      INT  "
                        + to_string(itr->getIntNum2()) + ","
                        + str_tmp + " ; ";

                    os << str_tmp << std::endl;
                }


                break;
            }
            case FrameWindow::VERSION_SEND: //发送接收版本号

                os << "      BOW  2 ; " << std::endl;

                str_tmp = "      TX   "
                    + convertModuleNumToName(bodyFrame,itr->getMainLRM())
                    + " VERSION ; ";
                os << str_tmp << std::endl;


                //后备模块1
                str_tmp = itr->getSupportLRM1();
                str_tmp = trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                + convertModuleNumToName(bodyFrame,itr->getSupportLRM1())
                                + " VERSION ; ";

                    os << str_tmp << std::endl;


                }

                //后备模块2
                str_tmp = itr->getSupportLRM2();
                str_tmp = trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                + convertModuleNumToName(bodyFrame, itr->getSupportLRM2())
                                + " VERSION ; ";

                    os << str_tmp << std::endl;

                }

                //后备模块3
                str_tmp = itr->getSupportLRM3();
                str_tmp = trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                               + convertModuleNumToName(bodyFrame,itr->getSupportLRM3())
                               + " VERSION ; ";

                    os << str_tmp << std::endl;

                }

                //接收模块
                for(auto x : itr->getReceiveLRMList()){
                    str_tmp = "      RX   "
                                + convertModuleNumToName(bodyFrame, x) + " "
                                + itr->getReceiveAddr() + " ;";

                    os << str_tmp << std::endl;
                }
//                for (int k = 0; k < itr->m_ReceiveLRMList.Count; k++)
//                {
//                    str_tmp = "      RX   "
//                                + convertModuleNumToName(bodyFrame,itr->m_ReceiveLRMList[k]) + " "
//                                + itr->m_strReceveAddr + " ;";
//                    os << str_tmp << std::endl;
//                }

                break;


            case FrameWindow::LONG_SYNC://发送长同步

                if (itr->getFlag())
                {//带版本校验

                    str_tmp = "      ERV  "
                        + itr->getSyncCode() + " , "
                        + convertModuleNumToName(bodyFrame,itr->getMainLRM()) + "  "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM1()) + " "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM2()) + "  "
                        +convertModuleNumToName(bodyFrame, itr->getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;

                }
                else
                {//不带版本校验
                    str_tmp = "      ERU  "
                     + itr->getSyncCode() + ",  "
                     + convertModuleNumToName(bodyFrame,itr->getMainLRM())+ "  "
                     +convertModuleNumToName(bodyFrame, itr->getSupportLRM1()) + " "
                     +convertModuleNumToName(bodyFrame,itr->getSupportLRM2()) + "  "
                     + convertModuleNumToName(bodyFrame,itr->getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;
                }

                break;

            case FrameWindow::FRAME_SWITCH://发送帧切换



                if (itr->getFlag())
                {//带版本校验

                    str_tmp = "      FCV  "
                        + itr->getSyncCode() + "  "
                        + itr->getNewFrameID() + "  "
                        + convertModuleNumToName(bodyFrame,itr->getMainLRM()) + "  "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM1()) + " "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM2() )+ "  "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM3() )+ " ;";

                    os << str_tmp << std::endl;

                }
                else
                {//不带版本校验
                    str_tmp = "      FCU  "
                        + itr->getSyncCode() + "  "
                        + itr->getNewFrameID() + "  "
                        + convertModuleNumToName(bodyFrame,itr->getMainLRM())+ "  "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM1()) + " "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM2()) + "  "
                        + convertModuleNumToName(bodyFrame,itr->getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;
                }

                break;

            case FrameWindow::CALL_SUBFRRAME://调用子帧


                if (itr->getFlag())
                {//用命令CALLI调用子序列并发送固有空闲

                    str_tmp = "      CALLI  "
                        + itr->getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;

                }
                else
                {//
                    str_tmp = "      CALL  "
                      + itr->getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;
                }

                break;

            case FrameWindow::INT_SEND://发送中断

                //接收模块
                str_tmp = "";
                for(auto x : itr->getReceiveLRMList()){
                    str_tmp += convertModuleNumToName(bodyFrame, x) +  " ";
                }
//                for (int k = 0; k < itr->m_ReceiveLRMList.Count; k++)
//                {

//                    str_tmp += convertModuleNumToName(bodyFrame, itr->m_ReceiveLRMList[k]) +  " ";

//                }

                str_tmp = "      INT  " + std::to_string(itr->getIntNum1()) + "," + str_tmp + " ; ";

                os << str_tmp << std::endl;

                break;

            case FrameWindow::FRAME_JUMP:

                if (itr->getFlag())
                {//用命令JUMPI跳转,并发送固有空闲

                    str_tmp = "      JUMPI  "
                        + itr->getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;

                }
                else
                {//
                    str_tmp = "      JUMP  "
                      + itr->getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;
                }

                break;

            case FrameWindow::FREE:

                    str_tmp = "      FREE  "
                        + std::to_string(itr->getDataCount()) + " ; ";

                    os << str_tmp << std::endl;

                break;

            case FrameWindow::SHORT_SYNC:


                str_tmp = "      SSYNC ; ";

                os << str_tmp << std::endl;

                break;

        }



    }

    //子序列结束时候返回
    if (dataFrame.getDataFrameType() == DataFrame::Child)
    {
        if (dataFrame.getRetWithGap() == true)
        {
            os << " RETI ;" << std::endl; //返回并发送固有空闲
        }
        else{
            os << " RET ;" << std::endl; //返回不发送固有空闲
        }
    }
    return true;
}

bool CommandFile::createFrameFreeTimeDown(std::ostream &os, const DataFrame &dataFrame, const BodyFrame &bodyFrame, int nTotalFreeTime)
{

    string str_tmp = "";
    int i, j;

    int nTmp;
    FrameWindow::WindowType nWindowType;
    string str_supportLRM;
    int nGap, nDelta;
    nGap = bodyFrame.getMessageInterval();
    //nGap = bodyFrame.m_nGap;
    //nDelta = bodyFrame.m_nDelta;
    nDelta = bodyFrame.getArbitrationStepDuration();

    nTotalFreeTime = nTotalFreeTime - 5 - nGap;

    for (auto window : dataFrame.getFrameWindows())
    {

        nWindowType = window.getWindowType();
        switch (nWindowType)
        {
            case FrameWindow::DATA_SEND: //发送接收数据


                if (window.getIntNum1() > 15)
                {
                    str_tmp = " ";
                    str_supportLRM = trim(window.getSupportLRM1());
                    if (str_tmp != "")
                    {
                        str_tmp += convertModuleNumToName(bodyFrame, str_supportLRM);


                    }
                    str_supportLRM = trim(window.getSupportLRM2());
                    if (str_tmp != "")
                    {
                        str_tmp += convertModuleNumToName(bodyFrame, str_supportLRM);

                    }
                    str_supportLRM = trim(window.getSupportLRM3());
                    if (str_tmp != "")
                    {
                        str_tmp += convertModuleNumToName(bodyFrame, str_supportLRM);
                    }


                    str_tmp = "      INT  " + std::to_string(window.getIntNum1()) + ","
                        + convertModuleNumToName(bodyFrame, window.getMainLRM())
                        + str_tmp + " ; ";

                    os << str_tmp << std::endl;

                }

                str_tmp = "      BOW  " + to_string(window.getDataCount()) + " ;";
                os << str_tmp << std::endl;

                str_tmp = "      TX   "
                    + convertModuleNumToName(bodyFrame, window.getMainLRM())
                    + " " + window.getSendAddr() + " ;";


                os << str_tmp << std::endl;

                //后备模块1
                str_tmp = window.getSupportLRM1();
                trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                + convertModuleNumToName(bodyFrame, window.getSupportLRM1()) + " "
                                + window.getSendAddr() + " ;";

                    os << str_tmp << std::endl;


                }

                //后备模块2
                str_tmp = window.getSupportLRM2();
                trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                + convertModuleNumToName(bodyFrame, window.getSupportLRM2()) + " "
                                + window.getSendAddr() + " ;";

                    os << str_tmp << std::endl;

                }

                //后备模块3
                str_tmp = window.getSupportLRM3();
                trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                          + convertModuleNumToName(bodyFrame, window.getSupportLRM3()) + " "
                          + window.getSendAddr() + " ;";

                    os << str_tmp << std::endl;

                }

                //接收模块
                for (auto x : window.getReceiveLRMList())
                {

                    str_tmp = "      RX   "

                        + convertModuleNumToName(bodyFrame, x) + " "
                        + window.getReceiveAddr() + " ;";

                    os << str_tmp << std::endl;


                }




                if (window.getIntNum2() > 15)
                {

                    str_tmp = " ";

                    for (auto x : window.getReceiveLRMList())
                    {

                        str_tmp += " " + convertModuleNumToName(bodyFrame, x);

                    }

                    str_tmp = "      INT  "
                        + to_string(window.getIntNum2()) + ","
                        + str_tmp + " ; ";

                    os << str_tmp << std::endl;


                }


                break;

            case FrameWindow::VERSION_SEND: //发送接收版本号

                os << "      BOW  2 ; " << std::endl;
                //sw.WriteLine("      BOW  2 ; ");

                str_tmp = "      TX   "
                    + convertModuleNumToName(bodyFrame, window.getMainLRM())
                    + " VERSION ; ";
                os << str_tmp << std::endl;


                //后备模块1
                str_tmp = window.getSupportLRM1();
                trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                + convertModuleNumToName(bodyFrame, window.getSupportLRM1())
                                + " VERSION ; ";

                    os << str_tmp << std::endl;


                }

                //后备模块2
                str_tmp = window.getSupportLRM2();
                trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                                + convertModuleNumToName(bodyFrame, window.getSupportLRM2())
                                + " VERSION ; ";

                    os << str_tmp << std::endl;

                }

                //后备模块3
                str_tmp = window.getSupportLRM3();
                trimStart(str_tmp);
                if (str_tmp != "")
                {
                    str_tmp = "      TX   "
                               + convertModuleNumToName(bodyFrame, window.getSupportLRM3())
                               + " VERSION ; ";

                    os << str_tmp << std::endl;

                }

                //接收模块
                for(auto x : window.getReceiveLRMList()){
                    str_tmp = "      RX   "
                                + convertModuleNumToName(bodyFrame, x) + " "
                                + window.getReceiveAddr() + " ;";

                    os << str_tmp << std::endl;
                }
//                for (int k = 0; k < window.m_ReceiveLRMList.Count; k++)
//                {

//                    str_tmp = "      RX   "
//                                + convertModuleNumToName(bodyFrame, window.m_ReceiveLRMList[k]) + " "
//                                + window.m_strReceveAddr + " ;";

//                    os << str_tmp << std::endl;


//                }

                break;


            case FrameWindow::LONG_SYNC://发送长同步

                if (window.getFlag())
                {//带版本校验

                    str_tmp = "      ERV  "
                        + window.getSyncCode() + " , "
                        + convertModuleNumToName(bodyFrame, window.getMainLRM()) + "  "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM1()) + " "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM2()) + "  "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;

                }
                else
                {//不带版本校验
                    str_tmp = "      ERU  "
                     + window.getSyncCode() + ",  "
                     + convertModuleNumToName(bodyFrame, window.getMainLRM()) + "  "
                     + convertModuleNumToName(bodyFrame, window.getSupportLRM1()) + " "
                     + convertModuleNumToName(bodyFrame, window.getSupportLRM2()) + "  "
                     + convertModuleNumToName(bodyFrame, window.getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;
                }

                break;

            case FrameWindow::FRAME_SWITCH://发送帧切换

                if (window.getFlag())
                {//带版本校验

                    str_tmp = "      FCV  "
                        + window.getSyncCode() + "  "
                        + window.getNewFrameID() + "  "
                        + convertModuleNumToName(bodyFrame, window.getMainLRM()) + "  "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM1()) + " "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM2()) + "  "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;

                }
                else
                {//不带版本校验
                    str_tmp = "      FCU  "
                        + window.getSyncCode() + "  "
                        + window.getNewFrameID() + "  "
                        + convertModuleNumToName(bodyFrame, window.getMainLRM()) + "  "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM1()) + " "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM2()) + "  "
                        + convertModuleNumToName(bodyFrame, window.getSupportLRM3()) + " ;";

                    os << str_tmp << std::endl;
                }


                break;

            case FrameWindow::CALL_SUBFRRAME://调用子帧

                if (window.getFlag())
                {//用命令CALLI调用子序列并发送固有空闲

                    str_tmp = "      CALLI  "
                        + window.getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;

                }
                else
                {//
                    str_tmp = "      CALL  "
                      + window.getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;
                }


                break;

            case FrameWindow::INT_SEND://发送中断

                //接收模块
                str_tmp = "";
                for(auto x : window.getReceiveLRMList()){
                    str_tmp += convertModuleNumToName(bodyFrame, x) + " ";
                }

                str_tmp = "      INT  " + to_string(window.getIntNum1()) + "," + str_tmp + " ; ";

                os << str_tmp << std::endl;

                break;

            case FrameWindow::FRAME_JUMP:

                writeFreeTime(os, nTotalFreeTime, nGap);//写入空闲等待
                if (window.getFlag())
                {//用命令JUMPI跳转,并发送固有空闲

                    str_tmp = "      JUMPI  "
                        + window.getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;

                }
                else
                {//
                    str_tmp = "      JUMP  "
                      + window.getNewFrameID() + " ; ";

                    os << str_tmp << std::endl;
                }


                goto EndFrame;


            case FrameWindow::FREE:

                str_tmp = "";

                str_tmp = "      FREE " + to_string(window.getDataCount()) + ";";

                os << str_tmp << std::endl;
                break;

            case FrameWindow::SHORT_SYNC:

                os << "      SSYNC;" << std::endl;

                break;


        }



    }

    EndFrame:
    //子序列结束时候返回
    if (dataFrame.getDataFrameType() == DataFrame::Child)
    {
        if (dataFrame.getRetWithGap() == true)
        {
            os << " RETI ;" << std::endl;
        }
        else{
            os << " RET ;" << std::endl;
        }
    }

    return true;
}

void CommandFile::writeFreeTime(std::ostream &os, int nFreeTime, int nGap)
{
    int nTmp = nFreeTime;

    if (nFreeTime<=0)
    {
        return;
    }

    while (nTmp > (20000 + 5 + nGap))
    {
        os << "      FREE 20000 ;" << std::endl;
        nTmp = nTmp - 20000 - 5 - nGap;  //每20000个位长周期需要添加一个短同步
        os << "      SSYNC;" << std::endl;
    }
    if (nTmp > 5 + nGap)
    {
        os << "      FREE  " << nTmp - 5 - nGap << " ;" << std::endl;
        os << "      SSYNC;" << std::endl;
    }
    else
    {
        os << "      SSYNC;" << std::endl;
    }
}

std::string CommandFile::convertModuleNumToName(const BodyFrame &bodyFrame, const std::string strModuleNum)
{
    using std::string;
    string str_tmp = trim(strModuleNum);
    string str_ModuleName ="";
    string str_left;
    int nIndex;
    int nTmp;
    if (str_tmp == "")
    {
         return "";
    }
    str_tmp = str_tmp + " ";

    while ((nIndex = str_tmp.find_first_of(" ")) > 0)
    {
        str_left = str_tmp.substr(0, nIndex + 1);
        //str_left = str_tmp.Substring(0, nIndex);
        str_left = trim(str_left);
        nTmp = atoi(str_left.data());
        //nTmp = Convert.ToInt32(str_left);

        for(auto modulePair : bodyFrame.getModules()){
            if(nTmp == modulePair.second.getModuleNumber()){
                str_ModuleName = str_ModuleName + " " + modulePair.second.getModuleName();
            }
        }

        str_tmp = str_tmp.substr(nIndex + 1, str_tmp.length() - nIndex - 1);
    }

    return str_ModuleName;
}

std::string CommandFile::convertModuleNumToName(const BodyFrame &bodyFrame, const int strModuleNum)
{
    using std::string;
    string str_ModuleName = "";

    //接收模块已经存在
    for(auto modulePair : bodyFrame.getModules()){
        if(strModuleNum == modulePair.second.getModuleNumber()){
            str_ModuleName = " " + modulePair.second.getModuleName();
        }
    }

    return str_ModuleName;
}


void VarInit(COMPILE_STATUS& c_status)
{
    c_status.line_num = 0;
    c_status.error = 0;
    c_status.warning = 0;
    c_status.section = -1;
}
