#ifndef COMMANDFILE_H
#define COMMANDFILE_H

#include "project/proj659.h"
#include <fstream>
using std::string;
struct COMPILE_STATUS
{
    uint  line_num;      /* 行号 */
    uint label_flag;    /* 标号标志 */
    uint error;         /* 编译状态,报错 */
    uint warning;       /* 编译状态,警告 */
    int section;       /* 段编号 */
};
struct LABEL_TABLE
{
   string name;              /* 标号名称 */
   string section;           /* 标号所在section名称 */
   int line;      /* 行号 */
   int addr;      /* 标号位于二进制文件的地址 */
   uint sub;       /* 此标号是否标识子序列 */
   int nGap;       /*消息间隔*/
   int nDelta;       /*主后背仲裁*/
};
class CommandFile
{
public:
    static bool createCommandFile(const Proj659&);
    static bool compileCommandFile(const Proj659&);

private:
    CommandFile();
    static bool createBodyFrameInfo(std::ostream&, const BodyFrame&);
    static bool createCommand(std::ofstream&, const BodyFrame&);
    static int getTotalCyl(const BodyFrame&, const DataFrame&);
    static bool createFrameAverage(std::ostream&, const DataFrame&, const BodyFrame&, int);
    static bool createFrameFreeTimeDown(std::ostream&, const DataFrame&, const BodyFrame&, int);
    static void writeFreeTime(std::ostream&, int nFreeTime, int nGap);
    static std::string convertModuleNumToName(const BodyFrame&, const std::string);
    static std::string convertModuleNumToName(const BodyFrame&, const int);
};

#endif // COMMANDFILE_H
