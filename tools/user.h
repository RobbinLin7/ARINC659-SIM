#ifndef USER_H
#define USER_H
#include<string>
#include<list>
#include<vector>
typedef unsigned int uint;
using String = std::string;
enum CommandAddr
{
    //单一命令区地址
    SGBSCALEADDR = 0X0100,/* TIMESCALE地址 */
    SGBGAPADDR = 0X0101,  /* GAP地址 */
    SGBDELTAADDR = 0X0102, /* DELTA地址 */
    SGBVERADDR = 0X0103,  /* VER地址 */
     SGBINITWAITADDR = 0x0110,/* 初始化延时时间首地址 */
    SGBRESVADDR = 0X0130, /* 保留空间首地址 */
    SGBTABLEADDR = 0X0140, /* 命令表首地址 */
    VERSIONADDR = 0X8000,/* TX指令中地址栏如果是VISION的地址值 */
    /* 全命令区地址 */
    TABLEADDR = 0x0100,  /* 命令查找表首地址 */
    SECBASEADDR = 0X0110,/* 命令区首地址 */
};


/// <summary>
///指令数据界限
/// </summary>
enum DataLimint
{
    //单一命令区地址
    INITNUMBER = 0X1F,  /* 初始化模块数 */
    MAXFREETIMES = 20000,  /* FREE的时间间隙上限  */
    MAXBITS24 = 0XFFFFFF,
    MAXGAPTIMES =   9  ,   /* GAP的时间间隙上限 */
    MINGAPTIMES =  2  ,  /* GAP的时间间隙下限 */
    MAXDELTATIMES =  10 , /* DELTA的时间间隙上限 */
    MINDELTATIMES =  3 , //* DELTA的时间间隙下限 */

    MAXVERMINOR= 0XFF,  /* 次版本号上限 */
    MAXCABINETPOS =  0XF ,        /* 机架号上限 */

    MAXBOWSIZE   =    0X100,       /* BOW窗口尺寸上限 */
    MAXTXCOUNT  =     4   ,        /* BOW/TX/RX指令中,TX最大的数 */
    MAXRXCOUNT   =    32   ,       /* BOW/TX/RX指令中,RX最大的数 */
    MAXLRMNUMBER  =   0X1F  ,      /* LRM号上限 */
    MAXSYNCODE    =   0XFF   ,     /* 同步代码上限 */
    MAXERULRMNUM  =   4    ,       /* ERU/ERV指令中,lrm最多的个数 */
    MAXINTCODE = 0XFFFFFF,   /* INT的同步代码地址上限 */
    MAXINTLRMNUM  =   0X1F ,       /* INT指令中,lrm最多的个数 */
};



/// <summary>
///数据窗口类型
/// </summary>
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
    SYNC = 8,   /* 短同步窗口 */

};

/// <summary>
///659命令集
/// </summary>
namespace CommandType {
enum CommandType
{
    BOW = 0,
    CALL = 1,
    CALLI = 2,
    FRAME_SWITCH=3,
    BOW_VERSION = 4,
    LONGSYNC=5,
    FREE = 7,
    FRAME_JUMP = 8,
    RET = 10,
    RETI = 11,
    SSYNC = 12,
 };
}


/// <summary>
/// 代码编译状态结构体
/// </summary>
struct COMPILE_STATUS
{
    uint  line_num;      /* 行号 */
    uint label_flag;    /* 标号标志 */
    uint error;         /* 编译状态,报错 */
    uint warning;       /* 编译状态,警告 */
    int section;       /* 段编号 */
};


/// <summary>
/// 标号结构体
/// </summary>
struct LABEL_TABLE
{
   String name;              /* 标号名称 */
   String section;           /* 标号所在section名称 */
   int line;                 /* 行号 */
   int addr;                 /* 标号位于二进制文件的地址 */
   uint sub;                 /* 此标号是否标识子序列 */
   int nGap;                 /*消息间隔*/
   int nDelta;               /*主后背仲裁*/
};


/// <summary>
/// 同步代码记录数组结构体
/// </summary>
struct SYN_CODE_REG
{
    uint code;      /* 同步代码 */
    uint line;      /* 同步代码所在的行号 */
};

/// <summary>
/// 同步代码记录数组结构体
/// </summary>
struct SYN_COMMAND_REG
{
    uint error;      /* ERU/ERV/FCU/FCV指令语法检查状态 */
    std::vector<SYN_CODE_REG> s_code; /* 同步代码数组 */
};
#endif // USER_H
