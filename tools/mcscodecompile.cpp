#include "mcscodecompile.h"
#include <string>
typedef unsigned int uint;
typedef std::string String;

McsCodeCompile::McsCodeCompile()
{

}

void McsCodeCompile::CompileMcsCode()
{
    char*  temp = new char [33];              /* 32个二进制数字符 */
    unsigned char check_sum;             /* 校验和 */
    uint index;       /* 字符索引号 */
    uint number;
    uint count;         /* 行号 */
    uint uint_tmp;
    String ans_line;        /* 本行应写入的字符串 */
    String ans_tmp;

//    temp = "00000000000000000000000000000000".ToCharArray();

//    String strMifFile = m_strDir + "/" + m_strFlieName + ".mif";
//    String strMcsFile = m_strDir + "/" + m_strFlieName + ".mcs";

//    StreamReader srMif = new StreamReader(strMifFile);
//    StreamWriter swMsc = File.CreateText(strMcsFile);
}
