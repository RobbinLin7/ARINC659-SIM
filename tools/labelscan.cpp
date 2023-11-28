#include "labelscan.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "tools/trim.h"
#include "tools/globalfunc.h"

LabelScan::LabelScan(string strDir, string strFileName)
{

}

std::vector<LABEL_TABLE> LabelScan::ScanLabel(COMPILE_STATUS &status)
{
    std::vector<LABEL_TABLE> ArrayLabel;
    LABEL_TABLE initLable;
    String ans_tmp, ans_label;
    String section_num;
    int line_cnt;
    int offset = 0;
    int label_length = 0;
    int nGap = 2;
    int nDelta = 5;

    LoadSrcFlie();

    m_lstLst.push_back("");

    /* 记录预处理时间 */
    // 获取当前日期和时间
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // 格式化为字符串
    std::tm tm = *std::localtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    ans_tmp = ss.str();
    m_lstLst.push_back(ans_tmp);

    ans_tmp = "-- LabelScan......";
    m_lstLst.push_back(ans_tmp);

    section_num = "-1";

    //逐行扫表判断是否为标号
//    for(line_cnt = 0; line_cnt <  m_lstSrc.size(); line_cnt++)
    for(String ans_tmp : m_lstSrc)
    {
        /* 取字串 */
        ans_label = GetString(ans_tmp, ans_tmp, label_length);

        /* 是空行 */
        if (ans_label.empty())
        {
            continue;
        }

        /* 是SECTION,记录段号 */
        if(ans_label == "SECTION")
        {
            String ans_label_tmp =  GetString(ans_tmp, ans_tmp, label_length);
            if (ans_label_tmp.empty())
            {
                section_num = "-1";
            }
            else
            {
                section_num = ans_label_tmp;
            }


            nGap = 2;   //消息间隔默认值
            nDelta = 5;//仲裁时间默认值
            offset = 0;
            continue;
        }

        /* 是GAP, 记录消息间隔 */
        if (ans_label == "GAP")
        {
            String str_tmp = GetString(ans_tmp, ans_tmp, label_length);


            if (str_tmp.empty())
            {
                nGap = 2 ;
            }
            else
            {
                GET_NUM num = GlobalFunc::StringToInt(str_tmp);

                if (num.flag!=-1)
                {
                    nGap = static_cast<int>(num.num);
                }

            }
            offset = 0;
            continue;
        }
        /* 是DELTA, 记录消息间隔 */
        if (ans_label == "DELTA")
        {
            String str_tmp = GetString(ans_tmp, ans_tmp, label_length);


            if (str_tmp.empty())
            {
                nDelta = 5;
            }
            else
            {
                GET_NUM num = GlobalFunc::StringToInt(str_tmp);

                if (num.flag != -1)
                {
                    nDelta = static_cast<int>(num.num);
                }

            }
            offset = 0;
            continue;
        }


        int int_tmp =GlobalFunc::CmpKW(ans_label);


        if(int_tmp == -1)
        {
             /* 标号名非法 */
            if((ans_label[0] < 'A') || (ans_label[0] > 'Z'))
            {

                status.error++;
                String strTmp;
                int nTmpLine = line_cnt+1;
                int nRow = m_lstSrc[line_cnt].find(ans_label) + 1;

                //原系统中  第一个标号不合法时候异常，现修改Find(ans_label)函数
                strTmp = "[Warning] Line" + to_string(nTmpLine) + "line ( " + to_string(nRow) + " row) :Label name" + ans_label + " is invalid. ";
                m_lstLst.push_back(strTmp);
             }

            /* 标号重名 */
            for(int i = 0; i < ArrayLabel.size(); i++)
            {
                if(ans_label == ArrayLabel[i].name)
                {
                    status.error++;
                    string  strTmp;
                    int nTmpLine = line_cnt+1;
                    int nRow = m_lstSrc[line_cnt].find(ans_label)+1;

                    strTmp = "[ERROR] Line " + to_string(nTmpLine) + "line (" + to_string(nRow) + "row):Same label name at line" + to_string(ArrayLabel[i].line);

                    m_lstLst.push_back(strTmp);

                    continue;
                }
            }
            initLable.name = ans_label;
            initLable.line = line_cnt;
            initLable.addr = offset + static_cast<int>(CommandAddr::SGBTABLEADDR);
            initLable.section = section_num;
            initLable.sub = 0;


            /* 读取标号后指令,计算偏移地址 */
            ans_label = GetString(ans_tmp, ans_tmp, label_length);
            /* 标号后跟SUB关键字,标识该标号表示一个子序列 */
            if(ans_label == "SUB")
            {
                initLable.sub = 1;
                /* 读取SUB后指令,计算偏移地址 */
                ans_label = GetString(ans_tmp, ans_tmp, label_length);
            }

            int int_tmp1 = GlobalFunc::CmpKW(ans_label);
            /* 标号后缺少命令或标号后紧跟标号*/
            if ((ans_label.empty()) || (int_tmp1 == -1))
            {
                status.error++;

                string strTmp;
                int nTmpLine = line_cnt + 1;
                int nRow = m_lstSrc[line_cnt].find(ans_label) + ans_label.length();
                strTmp = "[ERROR] Line " + to_string(nTmpLine) + " (" + to_string(nRow) + " row)::Miss comand after line" + ans_label;

                m_lstLst.push_back(strTmp);
                continue;
            }
            else if (ans_label != "SSYNC")
            {
                status.error++;

                string strTmp;
                int nTmpLine = line_cnt + 1;

                strTmp = "[ERROR] Line " + to_string(nTmpLine) + " ::Command after label must be SSYNC";

                m_lstLst.push_back(strTmp);
                continue;

            }


            initLable.nGap = nGap;
            initLable.nDelta = nDelta;

            ArrayLabel.push_back(initLable);


            /* 标号后跟关键字,地址累加 */
            offset += int_tmp1;
            continue;

        }

        /* 是关键字,地址累加 */
        offset += int_tmp;
    }

    SaveLstFile();
    m_lstLst.clear();
    m_lstSrc.clear();
    return ArrayLabel;
}

void LabelScan::LoadSrcFlie()
{
    string strFile = m_strDir + "/" + m_strFlieName + ".src";
    if (strFile.length() == 0) return;
    m_lstSrc.clear();
    try
    {
        // Create an instance of StreamReader to read from a file.
        // The using statement also closes the StreamReader.
        std::ifstream is("strFile");
        while(is){
            String line;
            std::getline(is, line);
            m_lstSrc.push_back(line);
        }
        is.close();
    }
    catch (std::exception e)
    {
        // Let the user know what went wrong.
        std::cout << e.what() << std::endl;
    }
}

String LabelScan::GetString(String ans_sor, String &ans_left, int &length)
{
    ans_sor = trimStart(ans_sor);
    ans_left = ans_sor;

    /* 除了空格就是空字符串 */
    if(ans_left == "")
    {
        return ans_left;
    }
    int nIndex = ans_left.find_first_of(" ");
    string ans_tmp = ans_left.substr(0, nIndex);
    ans_left = ans_left.substr(nIndex + 1, ans_left.length() - nIndex - 1);
    length = ans_tmp.length();
    return ans_tmp;
}

void LabelScan::SaveLstFile()
{
    string path = m_strDir + "/" + m_strFlieName + ".lst";

    try
    {
        std::ofstream os(path);
        if(os){
            for(int i = 0; i < m_lstLst.size(); i++){
                os << m_lstLst[i] << std::endl;
            }
            os.flush();
            os.close();
        }
    }
    catch (std::exception e)
    {
        // Let the user know what went wrong.
        std::cerr << e.what() << std::endl;
    }
}
