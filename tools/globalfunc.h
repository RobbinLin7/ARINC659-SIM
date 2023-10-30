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
};

#endif // GLOBALFUNC_H
