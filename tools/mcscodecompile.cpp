#include "mcscodecompile.h"
#include "tools/globalfunc.h"
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
typedef unsigned int uint;
typedef std::string String;
using std::to_string;

McsCodeCompile::McsCodeCompile()
{

}

void McsCodeCompile::CompileMcsCode()
{
    char* temp = new char [33];              /* 32个二进制数字符 */
    char check_sum;             /* 校验和 */
    uint index;       /* 字符索引号 */
    uint number;
    uint count;         /* 行号 */
    uint uint_tmp;
    String ans_line;        /* 本行应写入的字符串 */
    String ans_tmp;

    strcpy(temp, "00000000000000000000000000000000");
    //temp = "00000000000000000000000000000000".ToCharArray();

    String strMifFile = m_strDir + "/" + m_strFlieName + ".mif";
    String strMcsFile = m_strDir + "/" + m_strFlieName + ".mcs";

    std::ifstream srMif(strMifFile);
    std::ofstream swMsc(strMifFile);
//    StreamReader srMif = new StreamReader(strMifFile);
//    StreamWriter swMsc = File.CreateText(strMcsFile);

    //swCoe.WriteLine("memory_initialization_radix=2;\n");
    //swCoe.WriteLine("memory_initialization_vector=\n");


    /* 第一段数据 */

    /* 第一段起始 */
    swMsc << ":020000040000FA\r\n";
    //swMsc.Write(":020000040000FA\r\n");
    /* 第一段数据 */
    for (count = 0; count <= 0xFFF; count++)
    {
        /* 数据行起始 */
        ans_line = "10";

        /* 地址 */
//        std::stringstream ss;
//        ss << std::hex << std::setfill('0') << std::setw(3) << count;
//        ans_tmp = ss.str();
//        std::transform(ans_tmp.begin(), ans_tmp.end(), ans_tmp.begin(), toupper);
        toHex(count, 3, ans_tmp);
        //ans_tmp = count.ToString("X3");
        ans_line += ans_tmp;

        /* 固定填"000" */
        ans_line += "000";
        /* 填数据 */
        for (number = 0; number < 4; number++)
        {
            /* 取32个二进制数字符 */
            for (index = 0; index < 32; index++)
            {
                temp[index] =(char)srMif.get();
             }
            /* 将源文件的'\n'取出 */
            char tem = (char)srMif.get();
            srMif.get();
            /* 取出的32个二进制数字符转换为8个16进制数字符 */
            uint_tmp = NumBin32ToNumHex8(temp);

            toHex(uint_tmp, 8, ans_tmp);
            //ans_tmp = uint_tmp.ToString("X8");
            ans_line += ans_tmp;
        }

        /* 校验和 */
        check_sum = 0;
        for (int i = 0; i <= 38; i = i + 2)
        {
            ans_tmp= "0X" + to_string(ans_line[i]) + to_string(ans_line[i + 1]);


            check_sum += static_cast<unsigned char>(GlobalFunc::StringToInt(ans_tmp).num);
        }



       // byte ntem = (unsigned char)(0 - check_sum);

        check_sum = (unsigned char)(0 - check_sum);

        toHex(check_sum, 2, ans_tmp);
        //ans_tmp= check_sum.ToString("X2");
        ans_line += ans_tmp;

        /* 起始标志和换行 */
        ans_line = ":" + ans_line + "\r\n";
        /* 写入 */
        swMsc << ans_line << std::endl;
        //swMsc << ans_line << std::endl;
    }




    /* 第二段数据 */

    /* 第一段起始 */
    swMsc << ":020000040001F9\r\n";
    //swMsc.Write(":020000040001F9\r\n");
    /* 第一段数据 */
    for (count = 0; count <= 0xFFF; count++)
    {
        /* 数据行起始 */
        ans_line = "10";

        /* 地址 */

        toHex(count, 3, ans_tmp);
//        ans_tmp = count.ToString("X3");
        ans_line += ans_tmp;

        /* 固定填"000" */
        ans_line += "000";
        /* 填数据 */
        for (number = 0; number < 4; number++)
        {
            /* 取32个二进制数字符 */
            for (index = 0; index < 32; index++)
            {
                temp[index] = (char)srMif.get();
            }
            /* 将源文件的'\n'取出 */
            char tem = (char)srMif.get();
            srMif.get();
            /* 取出的32个二进制数字符转换为8个16进制数字符 */
            uint_tmp = NumBin32ToNumHex8(temp);
            toHex(uint_tmp, 8, ans_tmp);
//            ans_tmp = uint_tmp.ToString("X8");
            ans_line += ans_tmp;
        }

        /* 校验和 */
        check_sum = 0;
        for (int i = 0; i <= 38; i = i + 2)
        {
            ans_tmp = "0X" + to_string(ans_line[i]) + to_string(ans_line[i + 1]);
            check_sum += static_cast<unsigned char>(GlobalFunc::StringToInt(ans_tmp).num);
        }

        check_sum = (unsigned char)(0 - check_sum);

        toHex(check_sum, 2, ans_tmp);
//        ans_tmp = check_sum.ToString("X2");
        ans_line += ans_tmp;

        /* 起始标志和换行 */
        ans_line = ":" + ans_line + "\r\n";
        /* 写入 */

        swMsc << ans_line << std::endl;
    }




    /* 第三段数据 */

    /* 第一段起始 */
    swMsc << ":020000040002F8\r\n";
    /* 第一段数据 */
    for (count = 0; count <= 0xFFF; count++)
    {
        /* 数据行起始 */
        ans_line = "10";

        /* 地址 */
        toHex(count, 3, ans_tmp);
//        ans_tmp = count.ToString("X3");
        ans_line += ans_tmp;

        /* 固定填"000" */
        ans_line += "000";
        /* 填数据 */
        for (number = 0; number < 4; number++)
        {
            /* 取32个二进制数字符 */
            for (index = 0; index < 32; index++)
            {
                temp[index] = (char)srMif.get();
            }
            /* 将源文件的'\n'取出 */
            char tem = (char)srMif.get();
            srMif.get();
            /* 取出的32个二进制数字符转换为8个16进制数字符 */
            uint_tmp = NumBin32ToNumHex8(temp);
            toHex(uint_tmp, 8, ans_tmp);
//            ans_tmp = uint_tmp.ToString("X8");
            ans_line += ans_tmp;
        }

        /* 校验和 */
        check_sum = 0;
        for (int i = 0; i <= 38; i = i + 2)
        {
            ans_tmp = "0X" + to_string(ans_line[i]) + to_string(ans_line[i + 1]);
            check_sum += static_cast<unsigned char>(GlobalFunc::StringToInt(ans_tmp).num);
        }

        check_sum = (unsigned char)(0 - check_sum);

        toHex(check_sum, 2, ans_tmp);
        //ans_tmp = check_sum.ToString("X2");
        ans_line += ans_tmp;

        /* 起始标志和换行 */
        ans_line = ":" + ans_line + "\r\n";
        /* 写入 */
        swMsc << ans_line << std::endl;
    }




    /* 第三段数据 */

    /* 第一段起始 */
    swMsc << ":020000040003F7\r\n";
    /* 第一段数据 */
    for (count = 0; count <= 0xFFF; count++)
    {
        /* 数据行起始 */
        ans_line = "10";

        /* 地址 */
        toHex(count, 3, ans_tmp);
        //ans_tmp = count.ToString("X3");
        ans_line += ans_tmp;

        /* 固定填"000" */
        ans_line += "000";
        /* 填数据 */
        for (number = 0; number < 4; number++)
        {
            /* 取32个二进制数字符 */
            for (index = 0; index < 32; index++)
            {
                temp[index] = (char)srMif.get();
            }
            /* 将源文件的'\n'取出 */
            char tem = (char)srMif.get();
            srMif.get();
            /* 取出的32个二进制数字符转换为8个16进制数字符 */
            uint_tmp = NumBin32ToNumHex8(temp);
            toHex(uint_tmp, 8, ans_tmp);
            //ans_tmp = uint_tmp.ToString("X8");
            ans_line += ans_tmp;
        }

        /* 校验和 */
        check_sum = 0;
        for (int i = 0; i <= 38; i = i + 2)
        {
            ans_tmp = "0X" + to_string(ans_line[i]) + to_string(ans_line[i + 1]);
            check_sum += static_cast<unsigned char>(GlobalFunc::StringToInt(ans_tmp).num);
        }


        check_sum = (unsigned char)(0 - check_sum);
        toHex(check_sum, 2, ans_tmp);
//        ans_tmp = check_sum.ToString("X2");
        ans_line += ans_tmp;

        /* 起始标志和换行 */
        ans_line = ":" + ans_line + "\r\n";
        /* 写入 */
        swMsc << ans_line << std::endl;
    }
    swMsc << ":00000001FF\r\n";
    //swMsc.Write(":00000001FF\r\n");
    swMsc.flush();
    srMif.close();
    swMsc.close();
}

uint McsCodeCompile::NumBin32ToNumHex8(char num32[])
{
    /* 局部变量定义 */
    uint temp;          /* 临时变量 */
    unsigned short index;       /* 转换位的下标数 */

    /* 程序 */
    temp = 0;
    for(index = 0; index < 32; index++)
    {
        if(num32[index] != '0')
        {
            temp += static_cast<uint>(pow(2, 31 - index));
        }
    }

    return temp;
}

void McsCodeCompile::toHex(int count, int setw, String& ans_tmp)
{
    std::stringstream ss;
    ans_tmp = "";
    ss << std::hex << std::setfill('0') << std::setw(setw) << count;
    ans_tmp = ss.str();
    std::transform(ans_tmp.begin(), ans_tmp.end(), ans_tmp.begin(), toupper);
}
