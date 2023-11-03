#ifndef COMMANDFILE_H
#define COMMANDFILE_H

#include "project/proj659.h"
#include "tools/globalfunc.h"
#include "tools/user.h"
#include <fstream>
using std::string;
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
    static void SaveCompileResult(COMPILE_STATUS status, const Proj659&);
};

#endif // COMMANDFILE_H
