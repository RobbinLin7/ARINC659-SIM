#ifndef GRAMCHECK_H
#define GRAMCHECK_H
#include <string>
#include <vector>
#include <list>
#include "tools/user.h"
using String = std::string;
using List = std::vector<String>;
using List_LABEL_TABLE = std::vector<LABEL_TABLE>;
using uint = unsigned int;
class GramCheck
{
public:
    GramCheck(String strDir, String strFileName);
    uint CheckGram(COMPILE_STATUS  status, List_LABEL_TABLE label);


private:
    List m_lstSrc;          //预处理结果src命令列表
    List m_lstLst;          //处理结果lst 命令列表
    String m_strDir;        //文件路径
    String m_strFlieName;   //文件名称
    uint m_nSsyncMissTime;
    uint m_nGap;
    uint m_nDelta;
    void LoadSrcFile();
    String GetString(String ans_sor, String& ans_left, int& length);
    String GetLine(List lines, int cnt, int direction);
    uint SECTIONGram(COMPILE_STATUS& status, std::vector<String> m_ListLst , String ans);
    uint ESECTIONGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint TIMESCALEGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint GAPGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint DELTAGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint VERGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint INITWAITGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint BOWGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint TXGram(COMPILE_STATUS& status, uint &lrm, uint &addr, String ans);
    uint RXGram(COMPILE_STATUS& status, uint &lrm, uint &addr, String ans);
    uint CALLGram(COMPILE_STATUS& status, String ans, List_LABEL_TABLE label);
    SYN_COMMAND_REG ERUGram(COMPILE_STATUS& status, String ans, SYN_COMMAND_REG command);
    SYN_COMMAND_REG FCUGram(COMPILE_STATUS& status, String ans, List_LABEL_TABLE label, SYN_COMMAND_REG command);
    uint FREEGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint INTGram(COMPILE_STATUS& status, List& m_ListLst, String ans);
    uint JUMPGram(COMPILE_STATUS& status, String ans, List_LABEL_TABLE label);
    uint RETGram(COMPILE_STATUS& status, String ans);
    uint SSYNCGram(COMPILE_STATUS& status, String ans);
    void SaveLstFile();
};

#endif // GRAMCHECK_H
