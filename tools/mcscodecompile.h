#ifndef MCSCODECOMPILE_H
#define MCSCODECOMPILE_H
#include<string>
using String = std::string;
using uint = unsigned int;
class McsCodeCompile
{
public:
    McsCodeCompile();
    void CompileMcsCode();
    McsCodeCompile(String strDir, String strFileName){
         m_strDir = strDir;
         m_strFlieName = strFileName;
     }
private:
    String m_strDir; //文件路径
    String m_strFlieName;//文件名称
    uint NumBin32ToNumHex8(char num32[]);
    void toHex(int count, int setw, String& ans_tmp);
};

#endif // MCSCODECOMPILE_H
