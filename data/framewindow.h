#ifndef FRAMEWINDOW_H
#define FRAMEWINDOW_H
#include<string>
#include<list>
#include<set>

class FrameWindow
{
public:
    enum WindowType
    {
        //659命令窗口类型
        DATA_SEND = 0,  /* 数据传送窗口 */
        VERSION_SEND = 1,  /* 版本校验窗口 */
        LONG_SYNC = 2,     /* 长同步窗口 */
        FRAME_SWITCH = 3, /* 帧切换窗口 */
        CALL_SUBFRRAME = 4,  /* 调用子帧窗口 */
        INT_SEND = 5,           /*中断发送窗口 */
        FRAME_JUMP = 6,   /* 帧跳转窗口*/
        FREE = 7,   /* 空闲等待窗口 */
        SHORT_SYNC = 8,   /* 短同步窗口 */
    };
    FrameWindow();
    FrameWindow(int windowId);
    bool getFlag() const;
    void setFlag(bool newFlag);

    WindowType getWindowType() const;
    void setWindowType(WindowType newWindowType);

    int getWindowId() const;
    void setWindowId(int newWindowId);

    int getMainLRM() const;
    void setMainLRM(int newMainLRM);

    int getDataCount() const;
    void setDataCount(int newDataCount);

    int getSendTimeScale() const;
    void setSendTimeScale(int newSendTimeScale);

    int getIntNum1() const;
    void setIntNum1(int newIntNum1);

    int getIntNum2() const;
    void setIntNum2(int newIntNum2);

    const std::string &getSupportLRM1() const;
    void setSupportLRM1(const std::string &newSupportLRM1);

    const std::string &getSupportLRM2() const;
    void setSupportLRM2(const std::string &newSupportLRM2);

    const std::string &getSupportLRM3() const;
    void setSupportLRM3(const std::string &newSupportLRM3);

    const std::string &getSendAddr() const;
    void setStrSendAddr(const std::string &newStrSendAddr);

    const std::string &getReceiveAddr() const;
    void setStrReceiveAddr(const std::string &newStrReceveAddr);

    const std::string &getSyncCode() const;
    void setStrSyncCode(const std::string &newStrSynCode);

    const std::string &getNewFrameID() const;
    void setStrNewFrameID(const std::string &newStrNewFrameID);

    const std::list<int> &getReceiveLRMList()const;
    void removeAllOfReceiveLRMList();
    void setReceiveLRMList(const std::list<int> &newReceiveLRMList);
    void addReceiveLRM(const int LRM_id);
    bool isExistInReceiveLRMList(const int LRM_id);

private:
    bool flag; //版本校验窗口中为版本校验标识,在调用子帧以及JUMP 指令中标识是否发送固有空闲
    WindowType windowType;
    int windowId; //窗口序号
    int mainLRM; //主lrm
    int dataCount;  //发送数据个数
    int sendTimeScale;
    int intNum1; //中断1常用于发送中断
    int intNum2; //中断2常用于接收中断
    std::string supportLRM1; //后备LRM1; 后备LRM 可以为空
    std::string supportLRM2; //后备LRM2; 2010.3.29 考虑后备LRM可为空,将后备LRM类型由int修改为CSring 方便生成命令表
    std::string supportLRM3; //后备LRM2;
    std::string sendAddr; //发送地址
    std::string receiveAddr; //接收地址
    std::string syncCode; //发送长同步时的同步码
    std::string newFrameID; //新帧标识
    mutable std::list<int> receiveLRMList; //接收模块列表
    std::set<int> receiveLRMSet;



};

#endif // FRAMEWINDOW_H
