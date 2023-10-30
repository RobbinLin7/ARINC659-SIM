#ifndef LABELSCAN_H
#define LABELSCAN_H
#include<string>
#include<list>
using std::string;
//using std::list<string> =
class LabelScan
{
public:
    LabelScan(string strDir, string strFileName);

private:
    string m_strDir; //文件路径
    string m_strFlieName;//文件名称
    //List<String> m_lstSrc; //预处理结果src命令列表
    //List<String> m_lstLst; //处理结果lst 命令列表
};

#endif // LABELSCAN_H
