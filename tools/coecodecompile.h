#ifndef COECODECOMPILE_H
#define COECODECOMPILE_H
#include <string>
using String = std::string;

class CoeCodeCompile
{
public:
    CoeCodeCompile();
    CoeCodeCompile(String strDir, String strFileName)
    {
        m_strDir = strDir;
        m_strFlieName = strFileName;
    }
    void CompileCoeCode();
private:
    String m_strDir; //文件路径
    String m_strFlieName;//文件名称
};

#endif // COECODECOMPILE_H
