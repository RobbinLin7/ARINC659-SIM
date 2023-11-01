#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H
#include<string>
typedef unsigned int uint;
typedef std::string String;
using std::string;
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
        Int64 temp1;
        Int64 num_temp;
        GET_NUM result;

        if (String.IsNullOrEmpty(str_num.Trim()))
        {
            result.flag = -1;
            result.num = 0XFFFFFFFF;
            return result;
        }

        num_temp = 0;
        result.num = 0;
        result.flag = 0;
        length = str_num.Length;
        str_num = str_num.ToUpper();

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
                if (!char.IsDigit(str_num[i]))
                {
                    result.flag = -1;
                    result.num = 0XFFFFFFFF;
                    return result;
                }
                i++;
            }
            num_temp = Convert.ToInt32(str_num);
            result.flag = 10;
        }

        if (num_temp > 0XFFFFFFFF)
        {
            result.flag = 32;
            result.num = 0XFFFFFFFF;
            return result;
        }

        result.num = Convert.ToUInt32(num_temp);
        return result;
    }
};

#endif // GLOBALFUNC_H
