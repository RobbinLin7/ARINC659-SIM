#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H
#include<string>
#include<algorithm>
#include"tools/trim.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
typedef unsigned int uint;
typedef std::string String;
using std::string;
using std::to_string;
struct GET_NUM
{
    uint num;   /* 取出的数字 */
    int flag;           /* 数字标志,10为十进制,16为十六进制,-1非数字,32数字超过32位 */
};

class GlobalFunc
{
public:
    GlobalFunc();
    static int CmpKW(String ans)
    {
        if (CmpKWOthers(ans) == 1)
            return 0;
        if (CmpKW32(ans) == 1)
            return 1;
        if (CmpKW64(ans) == 1)
            return 2;
        if (CmpKW128(ans) == 1)
            return 4;

        return -1;

    }

    static int CmpKWOthers(String ans)
    {
        string KeyWordOthers[] =
        { "ESECTION", "SECTION", "GAP", "DELTA", "VER", "END", "SUB", "TX", "RX", "INITWAITTIME", "TIMESCALE" };

        for (int temp = 0; temp < 11; temp++)
        {
            if (ans == KeyWordOthers[temp])
            {
                return 1;
            }
        }
        return 0;
    }

    static int CmpKW32(String ans)
    {
          string KeyWord32[] =
          { "FREE", "SSYNC", "JUMP", "CALL", "RET", "JUMPI", "CALLI", "RETI" };

          for (int temp = 0; temp < 8; temp++)
          {
              if (ans == KeyWord32[temp])
              {
                  return 1;
              }
          }
          return 0;
    }

    static int CmpKW64(String ans)
    {
        string KeyWord64[] = { "ERU", "FCU", "ERV", "FCV", "INT" };

        for (int temp = 0; temp < 5; temp++)
        {
            if (ans == KeyWord64[temp])
            {
                return 1;
            }
        }
        return 0;
    }

    static int CmpKW128(String ans)
    {
        string KeyWord128[] = { "BOW" };

        for (int temp = 0; temp < 1; temp++)
        {
            if (ans == KeyWord128[temp])
            {
                return 1;
            }
        }
        return 0;
    }

   static GET_NUM StringToInt(String str_num)
   {
        int i;
        int length;
        int64_t temp1;
        int64_t num_temp;
        GET_NUM result;
//String.IsNullOrEmpty(str_num.Trim())
        if (trim(str_num).empty())
        {
            result.flag = -1;
            result.num = 0XFFFFFFFF;
            return result;
        }

        num_temp = 0;
        result.num = 0;
        result.flag = 0;
        length = str_num.length();
        std::transform(str_num.begin(), str_num.end(), str_num.begin(), toupper);
        if (length >= 2 && (str_num[0] == '0') && (str_num[1] == 'X'))
        {   /* 十六进制情况 */
            i = 2;
            while (i < length)
            {
                if ((str_num[i] >= '0') && (str_num[i] <= '9'))
                {
                    temp1 = str_num[i] - '0';
                    num_temp += (temp1 << ((length - i - 1) * 4));
                }
                else if ((str_num[i] <= 'F') && (str_num[i] >= 'A'))
                {
                    temp1 = str_num[i] - 55;
                    num_temp += (temp1 << ((length - i - 1) * 4));
                }
                else
                {
                    result.flag = -1;
                    result.num = 0XFFFFFFFF;

                    return result;
                }
                i++;
            }
            result.flag = 16;
        }
        else
        {   /* 十进制情况 */
            i = 0;
            while (i < length)
            {
                if(std::isdigit(str_num[i]) == false){
                    result.flag = -1;
                    result.num = 0XFFFFFFFF;
                    return result;
                }
                i++;
            }
            num_temp = atoi(str_num.c_str());
            result.flag = 10;
        }

        if (num_temp > 0XFFFFFFFF)
        {
            result.flag = 32;
            result.num = 0XFFFFFFFF;
            return result;
        }
        result.num = static_cast<uint32_t>(num_temp);
        //result.num = Convert.ToUInt32(num_temp);
        return result;
    }

   static std::string getDate(){
       /* 记录预处理时间 */
       // 获取当前日期和时间
       std::string ans_tmp;
       std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
       std::time_t now_c = std::chrono::system_clock::to_time_t(now);

       // 格式化为字符串
       std::tm tm = *std::localtime(&now_c);
       std::stringstream ss;
       ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
       ans_tmp = ss.str();
       return ans_tmp;
   }

   /// <summary>
   /// 返回错误信息
   /// </summary>
   /// <param name="ans;给定的字符串 "></param>
   /// <returns>返回格式化后的 错误字符串  </returns>
   static String ERRORFORMAT(int nLine, int nRow, String message)
   {
       String strError;
       strError = "[ERROR] Line " + to_string(nLine) + "(" + to_string(nRow) + "): " + message;
       return strError;
   }

   /// <summary>
   /// 返回警告信息
   /// </summary>
   /// <param name="nLine"></param>
   /// <param name="nRow"></param>
   /// <param name="message"></param>
   /// <returns></returns>
   static String WARNINGFORMAT(int nLine, int nRow, String message)
   {
       String strError;
       strError = "[Warning] Line " + to_string(nLine) + "(" + to_string(nRow) + "): " + message;
       return strError;
   }



   /// <summary>
   /// 从一个String中取一个字串
   /// </summary>
   /// <param name="ans_sor"></param>
   /// <param name="ans_left"></param>
   /// <param name="length"></param>
   /// <returns></returns>
   static String GetString(String ans_sor, String& ans_left, int& length)
   {
       ans_sor = trimStart(ans_sor);
       ans_left = ans_sor;

       /* 除了空格就是空字符串 */
       if (ans_left.empty() == true)
       {

           length = 0;
           return ans_left;

       }

       int nIndex = ans_left.find_first_of(" ");
       string ans_tmp = ans_left.substr(0, nIndex);
       ans_left = ans_left.substr(nIndex + 1, ans_left.length() - nIndex - 1);
       length = ans_tmp.length();

       return ans_tmp;
   }

   static uint CutLabel(String& ans)
   {
       uint row_cnt = 0;
       int label_length;  //uint label_length报错
       String ans_tmp;
       String ans_last_tmp;

       GetString(ans, ans, label_length);
       row_cnt += label_length;
       ans_tmp = GetString(ans, ans_last_tmp, label_length);
       if (ans_tmp == "SUB")
       {
           GetString(ans, ans, label_length);
           row_cnt += label_length;
       }
       return row_cnt;
   }

};

#endif // GLOBALFUNC_H
