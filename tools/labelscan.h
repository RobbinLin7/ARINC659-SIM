#ifndef LABELSCAN_H
#define LABELSCAN_H
#include<string>
#include<list>
#include"tools/user.h"
using std::string;
class LabelScan
{
public:
    LabelScan(string strDir, string strFileName);
    std::vector<LABEL_TABLE> ScanLabel(COMPILE_STATUS&  status);

private:
    string m_strDir; //文件路径
    string m_strFlieName;//文件名称
    void LoadSrcFlie();
    std::vector<String> m_lstSrc; //预处理结果src命令列表
    std::vector<String> m_lstLst; //处理结果lst命令列表
    String GetString(String ans_sor, String& ans_left, int& length);
    void SaveLstFile();
};

#endif // LABELSCAN_H
