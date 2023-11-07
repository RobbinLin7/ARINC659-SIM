#ifndef CODEGENERATE_H
#define CODEGENERATE_H
#include<string>
#include<vector>
#include "tools/globalfunc.h"
#include "tools/user.h"
using String = std::string;
using List = std::vector<std::string>;

class CodeGenerate
{
public:
    CodeGenerate(String strDir, String strFileName)
    {
        m_strDir = strDir;
        m_strFlieName = strFileName;
    }
    uint GenerateCode(COMPILE_STATUS status, std::vector<LABEL_TABLE> label);

private:
    String m_strDir; //文件路径
    String m_strFlieName;//文件名称
    List m_lstSrc; //预处理结果src命令列表
    List m_lstLst; //处理结果lst 命令列表
    List m_LstMif;
    void LoadSrcFlie();
    void MifInit();
    int FRAMEComp(uint baseline, std::vector<LABEL_TABLE> label);
    void BinaryNum(uint num, char num_array[]);
    void ESECTIONComp();
    void TIMESCALEComp(String ans, uint addr);
    void GAPComp(String ans, uint addr);
    void DELTAComp(String ans, uint addr);
    void VERComp(String ans, uint addr);
    void INITWAITTIMEComp(String ans, int addr);
    int BOWComp(String ans, int line);
    void CALLComp(String ans, std::vector<LABEL_TABLE> label);
    void CALLIComp(String ans, std::vector<LABEL_TABLE> label);
    uint ERUComp(String ans);
    uint ERVComp(String ans);
    void FCUComp(String ans, std::vector<LABEL_TABLE> label);
    void FCVComp(String ans, std::vector<LABEL_TABLE> label);
    void FREEComp(String ans);
    void INTComp(String ans);
    void JUMPComp(String ans, std::vector<LABEL_TABLE> label);
    void JUMPIComp(String ans, std::vector<LABEL_TABLE> label);
    void RETComp();
    void RETIComp();
    void SSYNCComp();
    void SaveMFile();
    void SaveMifFile(int section_num, int frame_length, int64_t mif_length);
    void SaveLstFile();

};

#endif // CODEGENERATE_H
