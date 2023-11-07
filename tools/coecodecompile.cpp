#include "coecodecompile.h"
#include "fstream"


CoeCodeCompile::CoeCodeCompile()
{

}

void CoeCodeCompile::CompileCoeCode()
{
    char temp;
    uint64_t count = 0;

    /* 生成.coe文件 */
    String strMifFile = m_strDir + "/" + m_strFlieName + ".mif";
    String strCoeFile = m_strDir + "/" + m_strFlieName + ".coe";

    std::ifstream is(strMifFile);
    std::ofstream os(strCoeFile);
//    StreamReader srMif = new StreamReader(strMifFile);
//    StreamWriter swCoe = File.CreateText(strCoeFile);
    if(is && os){
        os << "memory_initialization_radix=2;" << std::endl;
        os << "memory_initialization_vector=" << std::endl;
        while ((++count)>0)
        {
            //++count;
            is.get(temp);
            //temp = (char) srMif.Read();
            if(temp == '\r')
            {
                temp = is.get();
                os.write("\r\n", 2);
                //swCoe.Write(",\r\n");
            }
            else
            {
                os.write(&temp, 1);
            }

            if(count == (33*0XFFFF + 32))
            {
                break;
            }
        }
        os << ";";
        os.flush();
        is.close();
        os.close();
    }
//    swCoe.WriteLine("memory_initialization_radix=2;");
//    swCoe.WriteLine("memory_initialization_vector=");


    //srMif.Peek() >= 0

}
