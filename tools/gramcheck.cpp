#include "gramcheck.h"
#include <iostream>
#include <fstream>
#include "tools/globalfunc.h"
using std::to_string;


GramCheck::GramCheck(String strDir, String strFileName)
{
    this->m_strDir = strDir;
    this->m_strFlieName = strFileName;
}

String GramCheck::GetLine(List lines, int cnt, int direction)
{
    String ans;
    int  nsize = lines.size();
    if((direction == 0) || (nsize == 0))
    {
        return "";
    }

    direction = direction / abs(direction);

    while(true)
    {
        ans = lines[cnt];
        ans = trim(ans);
        if(ans.empty() == true)
        {
            /* 向上取且行号为0,向下取且行号为最大 */
            if(((cnt == 0) &&(direction == -1))
            || ((cnt == (nsize - 1)) && (direction == 1)))
            {
                return "";

            }
            cnt += direction;
        }
        else
        {
            return lines[cnt];
        }
    }
}

uint GramCheck::CheckGram(COMPILE_STATUS status, List_LABEL_TABLE label)
{
    uint error = 0;
    int label_cnt;              /* 标号计数器 */
    int sub_cnt;                /* 子序列数 */
    uint delta_flag;    /* DELTA唯一判断标志 */
    uint gap_flag;      /* GAP唯一判断标志 */
    uint initwait_flag; /* INITWAITTIME唯一判断标志 */
    uint section_flag;  /* 新段标志 */
    uint timescale_flag;/* TIMESCALE唯一判断标志 */
    uint ver_flag;      /* VER唯一判断标志 */
    int label_length;  /* 标号长 */
    int line_num;      /* 行号 */
    uint tx_addr=0;       /* TX的地址 */
    uint rx_addr =0;       /* RX的地址 */
    uint bow_level = 0;

    m_nSsyncMissTime = 0;

    bool bTmp;
    SYN_COMMAND_REG syn_command;/* 长同步指令寄存器 */
    syn_command.error = 1;

    String ans_tmp, str_tmp, ans_last_tmp;
    String strInfo;
    LoadSrcFile();

    /* 记录语法检查时间 */
    m_lstLst.push_back("");
//    DateTime currentTime = new DateTime();
//    currentTime = DateTime.Now;
//    ans_tmp = to_string(currentTime);
    m_lstLst.push_back(GlobalFunc::getDate());

    ans_tmp = "-- GramCheck......";
    m_lstLst.push_back(ans_tmp);

    label_cnt = 0;
    sub_cnt = 0;
    delta_flag = 0;
    gap_flag = 0;
    initwait_flag = 0;
    section_flag = 0;
    timescale_flag = 0;
    ver_flag = 0;
    line_num = 0;
    auto itr = m_lstSrc.begin();
    bTmp = false;
    while (!bTmp)
    {
        str_tmp = *itr;
        str_tmp = trim(str_tmp);
        if (str_tmp.empty() == false)
            break;
        line_num++;
        ++itr;
    }
    //region 首行SECTION 判断
    ans_tmp = *itr;
    str_tmp = GetString(ans_tmp, ans_last_tmp, label_length);

    if(str_tmp != "SECTION")
    {
        error = 1;
        status.error++;

        int nTmpLine = line_num + 1;
        int nRow = itr->find(str_tmp) + 1;

        //原系统中  第一个标号不合法时候异常，现修改Find(ans_label)函数
        strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + std::to_string(nRow) + " row) :Line 1 should shart with SECTION command.";
        m_lstLst.push_back(strInfo);

    }
    //#endregion

    //#region 逐行语法检查
    do
    {
        ans_tmp = *itr;
        str_tmp = GetString(ans_tmp, ans_last_tmp, label_length);
        status.label_flag = 0;



        //#region 是标号,即不是关键字且非空
        if ((GlobalFunc::CmpKW(str_tmp) == -1) && str_tmp.empty() == false)
        {
            status.label_flag = 1;
            m_nSsyncMissTime = 0;
            label_cnt++;
            /* 搜索标号数组,检查是否有此标号(在标号扫描中完成) */

            /* 标号不能在END,SECTION,GAP,DELTA,VER等非命令关键字以及TX,RX命令关键字前,除了SUB */
            str_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);


            if (GlobalFunc::CmpKWOthers(str_tmp)==1)
            {
                /* 是SUB命令 */
                if(str_tmp == "SUB")
                {
                    sub_cnt++;
                    label_cnt--;
                    /* 取"SUB后的字符串,必须为命令 */
                    str_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
                    /* 不是关键字或不是非命令关键字 */
                    if ((GlobalFunc::CmpKW(str_tmp) == -1) || (GlobalFunc::CmpKWOthers(str_tmp) == 1) || str_tmp.empty())
                    {



                       if((str_tmp == "TX") || (str_tmp == "RX"))
                        {
                            int nTmpLine = line_num + 1;
                            int nRow = itr->find(str_tmp) + 1;

                            //原系统中  第一个标号不合法时候异常，现修改Find(ans_label)函数
                            strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + std::to_string(nRow) + " row) :The next command of SUB can't be TX or RX.";
                            m_lstLst.push_back(strInfo);
                        }
                        else
                        {
                            int nTmpLine = line_num + 1;
                            int nRow = itr->find(str_tmp) + 1;

                            //原系统中  第一个标号不合法时候异常，现修改Find(ans_label)函数
                            strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + to_string(nRow) + " row) :Miss command after SUB. ";
                            m_lstLst.push_back(strInfo);
                        }

                        error = 1;
                        status.error++;
                    }

                }
                else
                {
                    error = 1;
                    status.error++;

                    int nTmpLine = line_num + 1;
                    int nRow = itr->find(str_tmp) + 1;

                    //原系统中  第一个标号不合法时候异常，现修改Find(ans_label)函数
                    strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + to_string(nRow) + " row) :" + str_tmp + "is not a command.";
                    m_lstLst.push_back(strInfo);

                }
            }


        }
        //#endregion
        //#region SUB语法检查
        if(str_tmp == "SUB")
        {
            error = 1;
            status.error++;
            int nTmpLine = line_num + 1;
            int nRow = itr->find(str_tmp) + 1;

            //原系统中第一个标号不合法时候异常，现修改Find(ans_label)函数
            strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + to_string(nRow) + " row) SUB can't be the head of the line.";
            m_lstLst.push_back(strInfo);
            continue;
        }
        //END退出 //最后一行退出错误
        if (str_tmp == "END")
        {

            ans_tmp = GetLine(m_lstSrc, line_num - 1, -1);
            if (ans_tmp.empty())
            {
                error = 1;
                status.error++;

                int nTmpLine = line_num + 1;
                int nRow = m_lstSrc[line_num].find(str_tmp) + 1;

                //原系统中  第一个标号不合法时候异常，现修改Find(ans_label)函数
                strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + to_string(nRow) + " row) :Miss 659 code.";
                m_lstLst.push_back(strInfo);

                break;
            }
            str_tmp = GetString(ans_tmp, ans_last_tmp, label_length);

            /* END前一条指令不是ESECTION */
            if(str_tmp != "ESECTION")
            {
                error = 1;
                status.error++;

                int nTmpLine = line_num + 1;
                int nRow = m_lstSrc[line_num].find(str_tmp) + 1;

                strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( " + to_string(nRow) + " row) :Expect ESECTION before END.";
                m_lstLst.push_back(strInfo);

                break;
            }
            break;

        }
          /* 读到最后一行退出 */
        else if((int)line_num >= m_lstSrc.size())
        {
            error = 1;
            status.error++;
            int nTmpLine = line_num + 1;
            strInfo = "[ERROR] Line" + to_string(nTmpLine) + "line ( 0 row) :Miss END.";
            m_lstLst.push_back(strInfo);
            break;
        }

        if(str_tmp == "SECTION")
        {
            /* 连续两个SECTION之间没有ESECTION */
            if(section_flag == 1)
            {
                error = 1;
                status.error++;


              m_lstLst.push_back(GlobalFunc::ERRORFORMAT(line_num+1,m_lstSrc[line_num].find(str_tmp) + 1,("Miss ESECTION before SECTION.")));

            }
            /* 新段标志置位 */
            section_flag = 1;
            /* 取SECTION后字符串,SECTION指令后必须是TIMESCALE,GAP,DELTA或VER */
            ans_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            str_tmp = GetString(ans_tmp, ans_last_tmp, label_length);

            /* 不是TIMESCALE且不是GAP且不是DELTA且不是VER */
            if((str_tmp != "TIMESCALE") &
                (str_tmp != "GAP") &
                str_tmp != ("DELTA") &
                str_tmp != "VER")
            {
                error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(line_num + 1, m_lstSrc[line_num].find(str_tmp) + 1, ("The next line of SECTION should be TIMESCALE or GAP or DELTA or VER command. (SECTION [TIMESCALE] [GAP] [DELTA] VER [INITWAITTIME] labelname)")));

            }

            /* 新起一段,TIMESCALE、GAP、DELTA、VER和INITWAITTIME标志清零 */
            gap_flag = 0;
            delta_flag = 0;
            initwait_flag = 0;
            timescale_flag = 0;
            ver_flag = 0;

            /* 同步代码清零 */
            syn_command.s_code.clear();
            ans_tmp = GetLine(m_lstSrc, line_num, 1);
            status.line_num = (uint)line_num;
            error |= SECTIONGram(status, m_lstLst, ans_tmp);
            continue;
        }

         /* ESECTION语法检查 */
        if (str_tmp == "ESECTION")
        {
             /* 一对SECTION和ESECTION之间必须有VER */
            if(ver_flag == 0)
            {
                error = 1;
                status.error++;
               int  row_cnt = m_lstSrc[line_num].find(str_tmp);
               m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                             (int)(status.line_num + 1),
                             (int)(row_cnt),
                             ("A section must have a VER.")
                             ));
            }
              /* 连续两个ESECTION之间没有SECTION或代码开头直接是SECTION */
            if(section_flag == 0)
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Miss SECTION before ESECTION..")
                              ));

            }

            /* ESECTION后必须跟随SECTION或END */
            String ans_esection_tmp;
            ans_esection_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            ans_esection_tmp = GetString(ans_esection_tmp,  ans_last_tmp,  label_length);

            if(!((ans_esection_tmp != "SECTION") || (ans_esection_tmp != "END")))
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Miss SECTION or END after ESECTION.")
                              ));

            }
            /* 新段标志清零 */
            section_flag = 0;
            status.line_num = (uint)line_num;
            error |= ESECTIONGram( status,  m_lstLst, ans_tmp);
            continue;
        }

        if(str_tmp == "TIMESCALE")
        {

            /* 在一个SECTION中只能有一个TIMESCALE */
            if(timescale_flag == 1)
            {
                error = 1;
                status.error++;


                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Only one TIMESCALE can be in a command table.")
                              ));

            }
            /* TIMESCALE标志置位 */
            timescale_flag = 1;
            /* 取TIMESCALE后字符串,TIMESCALE指令后必须是GAP,DELTA或VER */
            ans_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
              /* 不是GAP且不是DELTA且不是VER */
            if((!str_tmp.empty() == true)& (str_tmp!="GAP") & (str_tmp != "DELTA") & (str_tmp !="VER"))
            {
                  error = 1;
                  status.error++;
                  int row_cnt = m_lstSrc[line_num].find(str_tmp);

                  m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                (int)(status.line_num + 1),
                                (int)(row_cnt),
                                ("The next line of TIMESCALE should be GAP or DELTA or VER command. (SECTION [TIMESCALE] [GAP] [DELTA] VER [INITWAITTIME] labelname).")
                                ));
            }
            ans_tmp = GetLine(m_lstSrc, line_num, 1);
            status.line_num = (uint)line_num;
           error |= TIMESCALEGram( status,  m_lstLst, ans_tmp);
            continue;

        }

        if (str_tmp == "GAP")
        {
            /* 在一个SECTION中只能有一个GAP */
            if(gap_flag == 1)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Only one GAP can be in a command table.")
                              ));
            }
            /* GAP标志置位 */
            gap_flag = 1;
            /* 取TIMESCALE后字符串,TIMESCALE指令后必须是GAP,DELTA或VER */
            ans_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
             /* 不是DELTA且不是VER */
            if(!str_tmp.empty() == true & (str_tmp != "DELTA") & (str_tmp !="VER"))
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("The next line of GAP should be DELTA or VER command. (SECTION [TIMESCALE] [GAP] [DELTA] VER [INITWAITTIME] labelname)")
                              ));

            }

            ans_tmp = GetLine(m_lstSrc, line_num, 1);
            status.line_num = (uint)line_num;
            error |= GAPGram( status,  m_lstLst, ans_tmp);
            continue;




        }


        if(str_tmp == "DELTA")
        {
             /* 在一个SECTION中只能有一个DELTA */
            if(delta_flag == 1)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Only one DELTA can be in a command table.")
                              ));
            }
            delta_flag = 1;

            /* 取DELTA后字符串,DELTA指令后必须是VER  */
            ans_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
            /* 不是VER*/
            if (!str_tmp.empty() == true &  (str_tmp != "VER"))
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("The next line of DELTA should be VER command. (SECTION [TIMESCALE] [GAP] [DELTA] VER [INITWAITTIME] labelname))")
                              ));

            }
            ans_tmp = GetLine(m_lstSrc, line_num, 1);
            status.line_num = (uint)line_num;

            error |= DELTAGram( status,  m_lstLst, ans_tmp);
            continue;
        }


        if (str_tmp == "VER")
        {
            /*在一个SECTION中只能有一个VER */
            if (ver_flag == 1)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Only one VER can be in a command table.")
                              ));
            }
            ver_flag = 1;

            /* 取VER后字符串,VER指令后必须是INITWAITTIME或是label  */
            ans_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
            /* 不是INITWAITTIME且不是label */
            if (!str_tmp.empty() == true & (str_tmp !="INITWAITTIME") & (GlobalFunc::CmpKW(str_tmp) != -1))
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("The next line of VER should be INITWAITTIME command or a label. (SECTION [TIMESCALE] [GAP] [DELTA] VER [INITWAITTIME] labelname).")
                              ));

            }
            ans_tmp = m_lstSrc[line_num];
            status.line_num = (uint)line_num;

            error |= VERGram( status,  m_lstLst, ans_tmp);
            continue;
        }


        if (str_tmp == "INITWAITTIME")
        {
            /* 在一个SECTION中只能有一个INITWAITTIME */
            if (initwait_flag == 1)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Only one INITWAITTIME can be in a command table.")
                              ));
            }
            initwait_flag = 1;

            /* 取INITWAITTIME后字符串,INITWAITTIME指令后必须是标号 */
            ans_tmp = GetLine(m_lstSrc, line_num + 1, 1);
            str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
            /* 不是标号 */
            if (!str_tmp.empty() == true  & (GlobalFunc::CmpKW(str_tmp) != -1))
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("The next line of INITWAITTIME should start with a label. (SECTION [TIMESCALE] [GAP] [DELTA] VER [INITWAITTIME] labelname).")
                              ));

            }

            ans_tmp = GetLine(m_lstSrc, line_num, 1);
            status.line_num = (uint)line_num;
            error |= INITWAITGram( status,  m_lstLst, ans_tmp);
            continue;
        }


        if (str_tmp == "BOW")
        {

            status.line_num =(uint) line_num;
            error |= BOWGram( status,  m_lstLst, ans_tmp);

             /* 取BOW后字符串,BOW指令后必须是TX */
            uint *rtx_lrm = new uint[36];       /* RX和TX的lrm号数组 */
            int rtx_cnt = 0;
            int tx_cnt = 0;
            int cnt;
            cnt = line_num + 1;
            bow_level = 1;

            while (true)
            {
                /* 取下一条非空令 */
                while (true)
                {
                    ans_tmp =m_lstSrc[cnt];
                    trimStart(ans_tmp);
                    if(ans_tmp.empty() == true)
                    {
                        /* 向下取且行号为最大 */
                        if(cnt == m_lstSrc.size()-1)
                        {
                            ans_tmp = "";
                            break;
                        }
                        cnt++;
                    }
                    else
                    {
                        break;
                    }
                }
                str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
                 /* 若是RX指令,跳出 */
                if(str_tmp =="RX")
                {
                    break;
                }
                 /* 不是TX指令 */
                if(str_tmp !="TX")
                {
                    error = 1;
                    status.error++;

                    int row_cnt = m_lstSrc[line_num].find(str_tmp);

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                  (int)(status.line_num + 2),
                                  (int)(row_cnt),
                                  ("BOW/TX/RX command unexpect"+str_tmp+ ".")
                                  ));
                    break;
                }
                /* 是TX指令 */
                else
                {
                    tx_cnt++;
                    status.line_num = (uint)cnt;
                    uint tx_addr_tmp = tx_addr;
                    error |= TXGram( status, rtx_lrm[rtx_cnt], tx_addr, ans_tmp);

                    if(tx_cnt > 1)
                    {
                        /* TX的地址需要一致 */
                        if(tx_addr_tmp != tx_addr)
                        {
                            error = 1;
                            status.error++;
                            int row_cnt = m_lstSrc[line_num].find("TX");

                            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                 (int)(status.line_num + 1),
                                 (int)(row_cnt),
                                 ("The TX vendor_specs are not consistent (0x" + to_string(tx_addr_tmp) + ", 0x" + to_string(tx_addr) + ".")
                                 ));
                        }
                    }

                     /* TX,RX的所有lrm号都不能重复 */
                    int ii;
                    for(ii = 0; ii < rtx_cnt; ii++)
                    {
                        if(rtx_lrm[rtx_cnt] == rtx_lrm[ii])
                        {
                            error = 1;
                            status.error++;

                            int row_cnt = m_lstSrc[line_num].find("TX");

                            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                 (int)(status.line_num + 1),
                                 (int)(row_cnt),
                                 ("All TX/RX lrm number should be different.")
                                 ));
                        }
                    }
                    rtx_cnt++;
                }
                cnt++;

            }

            if(tx_cnt >static_cast<int>(DataLimint::MAXTXCOUNT))
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                     ("AThe TX number (" + to_string(tx_cnt) + ") is more than " + to_string(DataLimint::MAXTXCOUNT)+".")
                     ));

            }
            if (tx_cnt > 1) m_nSsyncMissTime += 3 * 9;

            if(tx_cnt == 0)
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                     ("Miss TX command.")
                     ));
            }

            /* 取TX后字符串,TX指令后必须是RX */
            int rx_cnt = 0;
            while (true)
            {
                  /* 取下一条非空令 */

                /* 取下一条非空令 */
                while (true)
                {
                    ans_tmp = m_lstSrc[cnt];
                    trimStart(ans_tmp);
                    if (ans_tmp.empty() == true)
                    {
                        /* 向下取且行号为最大 */
                        if (cnt == m_lstSrc.size() - 1)
                        {
                            ans_tmp = "";
                            break;
                        }
                        cnt++;
                    }
                    else
                    {
                        break;
                    }
                }
                str_tmp = GetString(ans_tmp,  ans_last_tmp,  label_length);
                /* 不是RX指令 */
                if (str_tmp != "RX")
                {
                    break;
                }
                /* 是RX指令 */
                else
                {

                    rx_cnt++;
                    status.line_num = (uint)cnt;
                    uint rx_addr_tmp = rx_addr;
                    error |= RXGram( status,  rtx_lrm[rtx_cnt],  rx_addr, ans_tmp);

                    if(rx_cnt > 1)
                    {
                        /* RX的地址需要一致 */
                        if(rx_addr_tmp != rx_addr)
                        {
                            error = 1;
                            status.error++;

                            int row_cnt = m_lstSrc[line_num].find("RX");

                            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                 (int)(status.line_num + 1),
                                 (int)(row_cnt),
                                 ("The TX vendor_specs are not consistent (0x" + to_string(rx_addr) + ", 0x" + to_string(rx_addr) + ".")
                                 ));

                        }
                    }

                             /* TX,RX的所有lrm号都不能重复 */
                    int ii;
                    for(ii = 0; ii < rtx_cnt; ii++)
                    {
                        if(rtx_lrm[rtx_cnt] == rtx_lrm[ii])
                        {
                            error = 1;
                            status.error++;
                            int row_cnt = m_lstSrc[line_num].find("RX");

                            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                 (int)(status.line_num + 1),
                                 (int)(row_cnt),
                                 ("All TX/RX lrm number should be different.")
                                 ));
                        }


                    }

                    rtx_cnt++;
                }
                cnt++;
            }

            if(rx_cnt >static_cast<int>(DataLimint::MAXRXCOUNT))
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                     ("AThe TX number (" + to_string(rx_cnt) + ") is more than " + to_string(DataLimint::MAXTXCOUNT) + ".")
                     ));
            }
            if(rx_cnt == 0)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                     ("Miss RX command.")
                     ));
            }
            line_num = cnt - 1;
            continue;
         }

         /* TX语法检查 */
        if(str_tmp =="TX")
        {

            if (bow_level ==0)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 2),
                              (int)(row_cnt),
                              ("BOW/TX/RX command unexpect.")
                              ));
                break;
            }

            bow_level = 2;
            continue;
        }


        if(str_tmp == "RX")
        {

            if (bow_level !=2)
            {
                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 2),
                              (int)(row_cnt),
                              ("BOW/TX/RX command unexpect.")
                              ));
                break;
            }
            continue;
        }

        bow_level = 0;






        if((str_tmp =="CALL") || (str_tmp =="CALLI"))
        {
            status.line_num = (uint)line_num;
            error |= CALLGram( status, ans_tmp, label);
            continue;
        }


         if((str_tmp =="ERU") || (str_tmp =="ERV"))
        {
            status.line_num = (uint)line_num;

            m_nSsyncMissTime =0;

            syn_command = ERUGram( status,  ans_tmp, syn_command);
            error |= syn_command.error;
            continue;
        }


        if((str_tmp =="FCU") || (str_tmp =="FCV"))
        {

            m_nSsyncMissTime = 0;
            status.line_num = (uint)line_num;
            syn_command = FCUGram( status, ans_tmp, label, syn_command);
            error |= syn_command.error;
            continue;
        }

         if(str_tmp =="FREE")
        {
            status.line_num = (uint)line_num;
            error |= FREEGram( status,  m_lstLst, ans_tmp);
            continue;
        }


        if(str_tmp =="INT")
        {

            if (m_nSsyncMissTime > 20000)
            {

                m_nSsyncMissTime = 0;
                error = 1;
                status.error++;
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                   0,
                   ("More than 20000 Miss SSYNC.")));
            }

            status.line_num =(uint) line_num;
            error |= INTGram( status,  m_lstLst, ans_tmp);
            continue;
        }

        if((str_tmp =="JUMP") || (str_tmp =="JUMPI"))
        {

            if (m_nSsyncMissTime>20000)
            {
                m_nSsyncMissTime = 0;
                error = 1;
                status.error++;
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                   0,
                   ("More than 20000 Miss SSYNC.")));
            }


            /* 前面只有一个标号与此JUMP成对,正常进行JUMP语法检查 */
            if(label_cnt == 1)
            {
                label_cnt = 0;
            }
            /* 前面没有一个标号与此JUMP成对 */
            else if(label_cnt == 0)
            {
                label_cnt = 0;
                error = 1;
                status.error++;


                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("Miss label befor JUMP/JUMPI.")));
            }

            else
            {

                error = 1;
                status.error++;

                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                              (int)(status.line_num + 1),
                              (int)(row_cnt),
                              ("More than one label(" + to_string(((int)(label_cnt - 1))) + ") befor JUMP/JUMPI.")));
                label_cnt = 0;
            }

            status.line_num = (uint)line_num;
            error |= JUMPGram( status, ans_tmp, label);
            continue;
        }

        if ((str_tmp == "RET") || (str_tmp == "RETI"))
        {
            if (m_nSsyncMissTime>20000)
           {
               m_nSsyncMissTime = 0;
               error = 1;
               status.error++;
               m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                  (int)(status.line_num + 1),
                  0,
                  ("More than 20000 Miss SSYNC.")));
           }


             /* 前面只有一个SUB与此RET成对,正常进行RET语法检查 */
            if(sub_cnt == 1)
            {
                sub_cnt = 0;
            }
             /* 前面没有一个SUB与此RET成对 */
            else if (sub_cnt == 0)
            {
                sub_cnt = 0;
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                            ("Miss label befor RET/RETI.")));
            }
            /* 前面超过一个SUB与此RET成对 */
            else
            {
                error = 1;
                status.error++;
                int row_cnt = m_lstSrc[line_num].find(str_tmp);

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                            ("More than one label( " + to_string(((int)(sub_cnt - 1))) + ") befor RET/RETI.")));
                sub_cnt = 0;
            }

            status.line_num =(uint) line_num;
            error |= RETGram( status, ans_tmp);
            continue;
        }


        if(str_tmp =="SSYNC")
        {

            m_nSsyncMissTime =0;
            status.line_num =(uint) line_num;
            error |= SSYNCGram( status, ans_tmp);
            continue;
        }
    }while ((line_num++)>=0);
}

uint GramCheck::SECTIONGram(COMPILE_STATUS &status, std::vector<String> m_ListLst, String ans)
{
    uint error = 0;
    uint row_cnt = 0;
    int  label_length;
    GET_NUM section_num;
    String ans_tmp, ans_last_tmp;



    status.section = -1;
    /* 如果前边有标号,取标号 */
    if (status.label_flag == 1)
    {
        row_cnt =GlobalFunc::CutLabel(ans);
        status.label_flag = 0;
    }

    /* 取SECTION */
    ans_tmp = GetString(ans,  ans_last_tmp,  label_length);
    row_cnt += (uint)label_length;
    /* 取section num */
    ans_tmp = GetString(ans_last_tmp,  ans_last_tmp,  label_length);
    row_cnt += (uint)label_length;
    ///* 缺少section num */
    if (ans_tmp.empty() == true)
    {
        status.error++;

        m_lstLst.push_back(GlobalFunc::ERRORFORMAT(static_cast<int>(status.line_num) + 1,
             (int)row_cnt,
            ("SECTION number's format ERROR.") ));
        return 1;
    }

    /* 转换section num 为数字 */
    section_num = GlobalFunc::StringToInt(ans_tmp);
    /* 若不是数字 */
    if(section_num.flag == -1)
    {
        error = 1;
        status.error++;

        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
        (int)(status.line_num + 1),
        (int)(row_cnt),
        ("SECTION number's format ERROR.")));


    }
    ///* section num 超限 */
    else if(section_num.num > 16)
    {
        error = 1;
        status.error++;

        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                ("SECTION number exceed 16.")
                ));

    }
    /* section num 不能为0 */
    else if(section_num.num == 0)
    {
        error = 1;
        status.error++;
        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                 (int)(status.line_num + 1),
                 (int)(row_cnt),
                   ("SECTION number can't be 0.")));
    }

    status.section = (int)section_num.num;
    /* section num 建议为十进制 */
    if(section_num.flag == 16)
    {
        status.warning++;


        m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                  ("SECTION number should be decimal.")));

    }

    ///* 取section num后的字符串 */
    ans_tmp = GetString(ans_last_tmp,  ans_last_tmp,  label_length);
    row_cnt += (uint)label_length;
    if (!ans_tmp.empty() == true)
    {
        error = 1;
        status.error++;


        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                  ("SECTION unexpect" + ans_tmp)));
    }


    return error;
}

uint GramCheck::ESECTIONGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{
    uint error = 0;
    uint row_cnt = 0;
    int label_length;

            String ans_tmp, ans_last_tmp;

    /* 如果前边有标号,取标号 */
    if(status.label_flag == 1)
    {
        row_cnt = GlobalFunc::CutLabel(ans);
        status.label_flag = 0;
    }


    /* 取ESECTION */
    ans_tmp = GetString(ans, ans_last_tmp, label_length);
    row_cnt +=(uint) label_length;
    /* 取ESECTION后的字符串 */
    ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
    row_cnt += (uint)label_length;


    if (!ans_tmp.empty() == true)
    {
        error = 1;
        status.error++;



        error = 1;
        status.error++;
        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                 (int)(status.line_num + 1),
                 (int)(row_cnt),
                   ("ESECTION unexpect " + ans_tmp)));


    }

    return error;
}

uint GramCheck::TIMESCALEGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{
    uint error = 0;
    uint row_cnt = 0;
    int label_length;

     String ans_tmp, ans_last_tmp;

    GET_NUM time_scale_value;
    /* 如果前边有标号,取标号 */
    if (status.label_flag == 1)
    {
        row_cnt = GlobalFunc::CutLabel( ans);
        status.label_flag = 0;
    }


    /* 取TIMESCALE */
    ans_tmp = GetString(ans, ans_last_tmp, label_length);
    row_cnt += (uint)label_length;
    /* 取time_scale_value */
    ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
    row_cnt += (uint)label_length;

    if (ans_tmp.empty() == true)
    {

        status.error++;
        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                 (int)(status.line_num + 1),
                 (int)(row_cnt),
                   ("Miss TIMESCALE time_scale_value.")));
        return 1;


    }

    /* 转换time_scale_value 为数字 */
    time_scale_value =GlobalFunc::StringToInt(ans_tmp);
    /* 若不是数字 */
    if(time_scale_value.flag == -1)
    {
        error = 1;
        status.error++;

        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                  ("TIMESCALE time_scale_value's format ERROR.")));

    }
     /* time_scale_value 超限,超过32位 */
    else if(time_scale_value.flag == 32)
    {
        error = 1;
        status.error++;

        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("TIMESCALE time_scale_value exceed 0xFFFFFFFF.")));
    }
     /* time_scale_value 建议为十进制 */
    if(time_scale_value.flag == 16)
    {
        status.warning++;

        m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("TIMESCALE time_scale_value should be decimal.")));

     }
     /* 取time_scale_value 后的字符串 */
     ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
     row_cnt += (uint)label_length;
     if (!ans_tmp.empty() == true)
     {
         error = 1;
         status.error++;
         m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
           (int)(status.line_num + 1),
           (int)(row_cnt),
             ("TIMESCALE unexpect "+ans_tmp)));
     }
     return error;
}

uint GramCheck::GAPGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{
    uint error = 0;
    uint row_cnt = 0;
    int label_length;

    String ans_tmp, ans_last_tmp;
    GET_NUM gap_times;
     /* 如果前边有标号,取标号 */
     if (status.label_flag == 1)
     {
         row_cnt = GlobalFunc::CutLabel( ans);
         status.label_flag = 0;
     }


     /* 取取GAP */
     ans_tmp = GetString(ans, ans_last_tmp, label_length);
     row_cnt += (uint)label_length;
     /* 取gap times */
     ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
     row_cnt += (uint)label_length;

     /* 缺少gap times */
     if (ans_tmp.empty() == true)
     {

         status.error++;
         m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                  (int)(status.line_num + 1),
                  (int)(row_cnt),
                    ("Miss GAP times.")));
         return 1;


     }

     /* 转换time_scale_value 为数字 */
     gap_times = GlobalFunc::StringToInt(ans_tmp);
     /* 若不是数字 */
     if (gap_times.flag == -1)
     {
         error = 1;
         status.error++;

         m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                 (int)(status.line_num + 1),
                 (int)(row_cnt),
                   ("GAP times's format ERROR.")));

     }
     /* gap times 超限 */
     else if ((gap_times.num > static_cast<uint>(DataLimint::MAXGAPTIMES)) || (gap_times.num < static_cast<uint>(DataLimint::MINGAPTIMES)))
     {
         error = 1;
         status.error++;

         m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("GAP times should be between " + to_string(DataLimint::MINGAPTIMES) + " and " + to_string(DataLimint::MAXGAPTIMES) +" .")));
     }
     /* gap times 建议为十进制 */
     if (gap_times.flag == 16)
     {
         status.warning++;

         m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("GAP times should be decimal.")));

     }
     /* 取gap times 后的字符串 */
     ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
     row_cnt += (uint)label_length;
     if (!ans_tmp.empty() == true)
     {
         error = 1;
         status.error++;
         m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
           (int)(status.line_num + 1),
           (int)(row_cnt),
             ("GAP unexpect  " + ans_tmp)));
     }
     return error;
}

uint GramCheck::DELTAGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{
    uint error = 0;
    uint row_cnt = 0;
    int label_length;


    String ans_tmp, ans_last_tmp;

    GET_NUM delta_times;
    /* 如果前边有标号,取标号 */
    if (status.label_flag == 1)
    {
        row_cnt = GlobalFunc::CutLabel( ans);
        status.label_flag = 0;
    }


    /* 取DELTA */
    ans_tmp = GetString(ans, ans_last_tmp, label_length);
    row_cnt += (uint)label_length;
    /* 取delta times */
    ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
    row_cnt += (uint)label_length;

    /* 缺少delta times */
    if (ans_tmp.empty() == true)
    {

        status.error++;
        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                 (int)(status.line_num + 1),
                 (int)(row_cnt),
                   ("Miss DELTA times.")));
        return 1;


    }

    /* 转换delta times 为数字 */
    delta_times = GlobalFunc::StringToInt(ans_tmp);
    /* 若不是数字 */
    if (delta_times.flag == -1)
    {
        error = 1;
        status.error++;

        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                  ("DELTA times's format ERROR.")));

    }
    /*  delta times 超限 */
    else if ((delta_times.num > static_cast<uint>(DataLimint::MAXDELTATIMES)) || (delta_times.num < static_cast<uint>(DataLimint::MINDELTATIMES)))
    {
        error = 1;
        status.error++;

        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("DELTA times should be between  " + to_string(DataLimint::MINDELTATIMES) + " and " + to_string(DataLimint::MAXDELTATIMES) + " .")));
    }
    /* delta times 建议为十进制 */
    if (delta_times.flag == 16)
    {
        status.warning++;

        m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("DELTA times should be decimal.")));

    }
    /* 取delta times 后的字符串 */
    ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
    row_cnt += (uint)label_length;
    if (!ans_tmp.empty() == true)
    {
        error = 1;
        status.error++;
        m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
          (int)(status.line_num + 1),
          (int)(row_cnt),
            ("DELTA unexpect " + ans_tmp)));
    }
    return error;
}

uint GramCheck::VERGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        GET_NUM ver_num;
        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取VER */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /* 取ver_value */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少ver_value、minor_ver、cabinet_pos */
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss VER value, minor_ver and cabinet_pos.")));
            return 1;


        }

        /* 转换ver_value 为数字 */
        ver_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (ver_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                      ("VER ver_value's format ERROR.")));

        }
        /*  ver_value 超限*/
        else if (ver_num.flag == 32)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                  (int)(status.line_num + 1),
                  (int)(row_cnt),
                    ("VER ver_value exceed 0XFFFFFFFF.")));
        }
        /* ver_value 建议为十六进制 */
        if (ver_num.flag == 10)
        {
            status.warning++;

            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
                  (int)(status.line_num + 1),
                  (int)(row_cnt),
                    ("VER ver_value should be hexadecimal.")));

        }
        /* 取minor_ver */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
         /* 缺少 缺少minor_ver、cabinet_pos */
        if (ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("Miss VER minor_ver and cabinet_pos.")));
            return 1;
        }
         /* 转换minor_ver 为数字 */
        ver_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if(ver_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("VER minor_ver's format ERROR.")));
        }
         /* minor_ver 超限 */
        else if (ver_num.num >static_cast<int>( DataLimint::MAXVERMINOR))
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                ("VER minor_ver exceed 0x" + to_string(DataLimint::MAXVERMINOR)+ ".")
                ));

        }

         /* minor_ver 建议为十六进制 */
        if(ver_num.flag == 10)
        {
            status.warning++;
            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
               ("VER minor_ver should be hexadecimal.")
               ));
        }
        /* 取cabinet_pos */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /* 缺少cabinet_pos */
        if (ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
               ("Miss VER cabinet_pos.")));
            return 1;
        }
        /* 转换cabinet_pos 为数字 */
        ver_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (ver_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("VER cabinet_pos's format ERROR.")));
        }
        /* cabinet_pos 超限 */
        else if (ver_num.num > static_cast<int>(DataLimint::MAXCABINETPOS))
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                (int)(status.line_num + 1),
                (int)(row_cnt),
                ("VER cabinet_pos exceed" + to_string(DataLimint::MAXCABINETPOS) + ".")
                ));

        }

          /* cabinet_pos 建议为十进制 */
        if(ver_num.flag == 16)
        {
            status.warning++;

            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
              ("VER cabinet_pos should be decimal.")
              ));

        }
        /* 取cabinet_pos 后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
            (int)(status.line_num + 1),
            (int)(row_cnt),
            ("Ver unexpect "+ans_tmp)
            ));

        }

        return error;


    }
}

uint GramCheck::INITWAITGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;
        int m_cnt = 0;


        String ans_tmp, ans_last_tmp;

        GET_NUM m_time;
        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取INITWAITTIME */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取m_time*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少m_times*/
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss INITWAITTIME module_initwaittime.")));
            return 1;


        }
        /* 取m_time序列 */
        while (true)
        {
            ++m_cnt;
            /* m_time数超过初始化模块数 */
            if(m_cnt > static_cast<int>(DataLimint::INITNUMBER + 1))
            {
                status.error++;

                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                      "INITWAITTIME module_initwaittime number("+ to_string(m_cnt) + ") should be less than " +
                      ((to_string(static_cast<int>(DataLimint::INITNUMBER))+"."))));
                return 1;
            }

            /* 转换m_time 为数字 */
            m_time = GlobalFunc::StringToInt(ans_tmp);
            /* 若不是数字 */
            if (m_time.flag == -1)
            {
                error = 1;
                status.error++;

                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                        (int)(status.line_num + 1),
                        (int)(row_cnt),
                          ("INITWAITTIME module_initwaittime's format ERROR.")));

            }
             /* m_time超限,超过32位 */
            else if(m_time.flag == 32)
            {
                error = 1;
                status.error++;

                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                        (int)(status.line_num + 1),
                        (int)(row_cnt),
                          ("INITWAITTIME module_initwaittime exceed 0xFFFFFFFF.")));
            }


             /* m time 建议为十六进制 */
            if(m_time.flag == 10)
            {
                status.warning++;
                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                       (int)(status.line_num + 1),
                       (int)(row_cnt),
                         ("INITWAITTIME module_initwaittime should be hexadecimal.")));
            }

            /* 取m_time */
            ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
            row_cnt += (uint)label_length;
            /* 是否行尾 */
            if (ans_tmp.empty() == true)
            {
                break;
            }
        }

        return error;
    }
}

uint GramCheck::BOWGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        GET_NUM bow_num;
        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取BOW */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取BOW size*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少BOW size*/
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss BOW size.")));
            return 1;


        }
        /*转换BOW size 为数字 */
        /* 转换delta times 为数字 */
        bow_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (bow_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                     ("BOW size's format ERROR.")));

        }
         /* BOW size 超限 */
        else if(bow_num.num >static_cast<int>(DataLimint::MAXBOWSIZE))
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("BOW size should exceed " + to_string(DataLimint::MAXBOWSIZE) +  " .")));

        }
         /* BOW size 不能为0 */
        else if(bow_num.num == 0)
        {
            error = 1;
            status.error++;


            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("BOW size can't be 0.")));
        }


        m_nSsyncMissTime += 16*(bow_num.num)+10;

        if (m_nSsyncMissTime>20000)
        {

            m_nSsyncMissTime = 0;
            error = 1;
            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               0,
               ("More than 20000 Miss SSYNC.")));
        }

         /* BOW size 建议为十进制 */
        if(bow_num.flag == 16)
        {
            status.warning++;
            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
             (int)(status.line_num + 1),
             (int)(row_cnt),
               ("BOW size should be decimal.")));

        }

        /* 取BOW size后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("BOW unexpect" + ans_tmp)));
        }

        return error;
    }
}

uint GramCheck::TXGram(COMPILE_STATUS &status, uint &lrm, uint &addr, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        GET_NUM tx_num;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }

        /* 取TX */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取lrm*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* /* 缺少lrm和vendor_spec */
        if (ans_tmp.empty() == true)
        {


            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss TX lrm and vendor_spec.")));
            return 1;

        }
        /* 转换delta times 为数字 */
        tx_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (tx_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                     ("TX lrm's format ERROR.")));

        }
        /* BOW size 超限 */
        else
        {
            lrm = tx_num.num;
            /* lrm 超限 */
            if(tx_num.num >static_cast<uint>( DataLimint::MAXLRMNUMBER))
            {
                error = 1;
                status.error++;
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                 ("TX lrm exceed " + to_string(DataLimint::MAXLRMNUMBER) + " .")));
            }

        }


          /* lrm 建议为十进制 */
        if(tx_num.flag == 16)
        {
            status.warning++;

            m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                          (int)(status.line_num + 1),
                          (int)(row_cnt),
               ("TX lrm should be decimal.")));


        }
        /* 取vendor_spec */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /* 缺少vendor_spec */
        if (ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("Miss TX vendor_spec or VERSION.")));
           return 1;
        }
        /* 若vendor_spec为VERSION */
        if (ans_tmp == "VERSION")
        {
            addr = static_cast<uint>(CommandAddr::VERSIONADDR);
            if (addr > static_cast<uint>(DataLimint::MAXBITS24))
            {
                error = 1;
                status.error++;
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                        (int)(status.line_num + 1),
                         (int)(row_cnt),
                         ("TX vendor_spec exceed 0XFFFFFF.")));

            }
        }
        else
        {
            /* 转换vendor_spec为数字 */
            tx_num = GlobalFunc::StringToInt(ans_tmp);
              /* 若不是数字 */
            if(tx_num.flag == -1)
            {
                error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                       (int)(status.line_num + 1),
                        (int)(row_cnt),
                        ("TX vendor_spec's format ERROR.")));

            }
             else
            {
                /* 保存TX指令的地址,以供与其它TX指令比较是否一致 */
               addr = tx_num.num;
                if(tx_num.num >static_cast<uint>(DataLimint::MAXBITS24))
                {
                    error = 1;
                    status.error++;

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                       (int)(status.line_num + 1),
                        (int)(row_cnt),
                        ("TX vendor_spec exceed "+to_string(DataLimint::MAXBITS24))));

                }

                /* vendor_spec 建议为十六进制 */
                if(tx_num.flag == 10)
                {
                    status.warning++;
                    m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                     (int)(status.line_num + 1),
                      (int)(row_cnt),
                      ("TX vendor_spec should be hexadecimal")));


                }
            }
        }

        /* 取label 后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("TX unexpect" + ans_tmp)));
        }

        return error;
    }
}

uint GramCheck::RXGram(COMPILE_STATUS &status, uint &lrm, uint &addr, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        GET_NUM rx_num;
        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取RX */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取lrm*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* /* 缺少lrm和vendor_spec */
        if (ans_tmp.empty() == true)
        {


            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss RX lrm and vendor_spec.")));
            return 1;


        }
        /* 转换delta times 为数字 */
        rx_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (rx_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                     ("RX lrm's format ERROR.")));

        }
        /* BOW size 超限 */
        else
        {
            lrm = rx_num.num;
            /* lrm 超限 */
            if (rx_num.num > static_cast<uint>(DataLimint::MAXLRMNUMBER))
            {
                error = 1;
                status.error++;
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                 ("RX lrm exceed " + to_string(DataLimint::MAXLRMNUMBER) + " .")));
            }

        }


        /* lrm 建议为十进制 */
        if (rx_num.flag == 16)
        {
            status.warning++;

            m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                          (int)(status.line_num + 1),
                          (int)(row_cnt),
               ("RX lrm should be decimal.")));


        }
        /* 取vendor_spec */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /* 缺少vendor_spec */
        if (ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("Miss RX vendor_spec .")));
            return 1;
        }

      /* 转换vendor_spec为数字 */
        rx_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (rx_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                    (int)(row_cnt),
                    ("TX vendor_spec's format ERROR.")));

        }
        else
        {
            /* 保存TX指令的地址,以供与其它TX指令比较是否一致 */
            addr = rx_num.num;
            if (rx_num.num > static_cast<uint>(DataLimint::MAXBITS24))
            {
                error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                    (int)(row_cnt),
                    ("RX vendor_spec exceed " + to_string(DataLimint::MAXBITS24))));

            }

            /* vendor_spec 建议为十六进制 */
            if (rx_num.flag == 10)
            {
                status.warning++;
                m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                 (int)(status.line_num + 1),
                  (int)(row_cnt),
                  ("RX vendor_spec should be hexadecimal")));


            }
        }


        /* 取label 后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("RX unexpect" + ans_tmp)));
        }

        return error;
    }
}

uint GramCheck::CALLGram(COMPILE_STATUS &status, String ans, List_LABEL_TABLE label)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取CALL/CALLI */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取label*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少label */
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss CALL/CALLI label.")));
            return 1;


        }
        /* 编号是关键字 */
        if(!(GlobalFunc::CmpKW(ans_tmp) == -1))
        {
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                   (int)(row_cnt),
                     ("CALL/CALLI label can't be a keyword.")));
            return 1;

        }
        /* 搜索标号数组,检查是否有此标号 */
        int i = 0;
        while (true)
        {
            if (label[i].name == ans_tmp)
            {
                /* 标号不在本段 */
                if(label[i].section != to_string(status.section))
                {
                    error = 1;
                    status.error++;

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                   (int)(status.line_num + 1),
                                   (int)(row_cnt),
                                     ("CALL/CALLI label "+ label[i].name + " is not in this section or is same as other label name.")));

                 }
                /* 本标号不是子序列 */
                if(label[i].sub != 1)
                {
                    error = 1;
                    status.error++;

                      m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                   (int)(status.line_num + 1),
                                   (int)(row_cnt),
                                     ("CALL/CALLI label"+ ans_tmp + " is not a subsequence.")));

                }
                break;

            }

            i++;
            if(i == label.size())
            {
                error = 1;
                status.error++;
                  m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                   (int)(status.line_num + 1),
                                   (int)(row_cnt),
                                     ("Can't find CALL/CALLI label "+ ans_tmp + " .")));
                break;
            }
        }

        /* 取label 后的字符串  */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("CALL/CALLI unexpect" + ans_tmp)));
        }

        return error;
    }
}

SYN_COMMAND_REG GramCheck::ERUGram(COMPILE_STATUS &status, String ans, SYN_COMMAND_REG command)
{

    {

        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        GET_NUM eru_num;
        SYN_CODE_REG command_reg;
        command.error = 0;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取ERU/ERV*/
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取code */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少code和lrm_list */
        if (ans_tmp.empty() == true)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss ERU/ERV code and lrm_list.")));
            return command;


        }

        /* 转换code 为数字 */
        eru_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if(eru_num.flag == -1)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                      ("ERU/ERV code's format ERROR.")));
        }
         /* code 超限 */
        else if(eru_num.num >static_cast<int>(DataLimint::MAXSYNCODE))
        {
            command.error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                   (int)(row_cnt),
                     ("ERU/ERV code exceed  " + to_string(DataLimint::MAXSYNCODE)+".")));
        }
         /* code 不能为0 */
        if(eru_num.num == 0)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("ERU/ERV code can't be 0.")));

        }
        /* 同步代码不能重复 */

        else
        {
            /* 搜索同步代码数组 */
            for(int i = 0;i < command.s_code.size(); i++)
            {
                if(command.s_code[i].code == eru_num.num)
                {
                    command.error = 1;
                    status.error++;

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                             (int)(status.line_num + 1),
                             (int)(row_cnt),
                               ("ERU/ERV code can't be equal to other syn_code( " + to_string(command.s_code[i].line) + ").")));
                }
            }
            /* 记录同步代码 */

            command_reg.code = eru_num.num;
            command_reg.line = status.line_num + 1;
            command.s_code.push_back(command_reg);
        }

        /* code 建议为十进制 */
        if(eru_num.flag == 16)
        {
            status.warning++;

            m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                              ("ERU/ERV code should be decimal.")));
        }
        /* 取',' */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少lrm_list */
        if (ans_tmp.empty() == true )
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                           (int)(status.line_num + 1),
                           (int)(row_cnt),
                             ("Miss ERU/ERV lrm_list")));
            return command;
        }
        /* 若不是',' */
        if (ans_tmp !=",")
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                           (int)(status.line_num + 1),
                           (int)(row_cnt),
                             ("Miss ERU/ERV ','.")));

        }
        else
        {
            /* 取lrm_list */
            ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
            row_cnt += (uint)label_length;
        }

        /* 缺少lrm  */
        if (ans_tmp.empty() == true)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                           (int)(status.line_num + 1),
                           (int)(row_cnt),
                             ("Miss ERU/ERV lrm_list.")));
            return command;
        }
        int lrm_num = 0;

        while (true)
        {
            lrm_num++;
            /* 超过4个lrm */
            if(lrm_num >static_cast<int>( DataLimint::MAXERULRMNUM))
            {
                command.error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                         (int)(status.line_num + 1),
                         (int)(row_cnt),
                           ("The ERU/ERV lrm number exceecd " + to_string(DataLimint::MAXERULRMNUM))));

            }
            /* 转换lrm 为数字 */
           eru_num = GlobalFunc::StringToInt(ans_tmp);
             /* 若不是数字 */
            if(eru_num.flag == -1)
            {
                command.error = 1;
                status.error++;
                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                        (int)(status.line_num + 1),
                        (int)(row_cnt),
                          ("TERU/ERV lrm's format ERROR")));
            }
              /* lrm 超限 */
            else if(eru_num.num >static_cast<int>( DataLimint::MAXLRMNUMBER))
            {
                command.error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                       (int)(status.line_num + 1),
                       (int)(row_cnt),
                         ("ERU/ERV lrm exceed " + to_string(DataLimint::MAXLRMNUMBER)+" .")));
            }

            /* lrm 建议为十进制 */
            if(eru_num.flag == 16)
            {
                status.warning++;

                m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                      (int)(status.line_num + 1),
                      (int)(row_cnt),
                        ("ERU/ERV lrm should be decimal.")));


            }

            /* 取下一个lrm */
            ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
            row_cnt += (uint)label_length;
            /* 没有多余的字符串 */
            if (ans_tmp.empty() == true)
            {
                return command;
            }

        }


    }
}

SYN_COMMAND_REG GramCheck::FCUGram(COMPILE_STATUS &status, String ans, List_LABEL_TABLE label, SYN_COMMAND_REG command)
{

    {

        uint row_cnt = 0;
        int label_length;
        int i = 0;

        String ans_tmp, ans_last_tmp;

        GET_NUM fcu_num;
        SYN_CODE_REG command_reg;
        command.error = 0;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取FCU/FCV*/
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取code */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少code flabel lrm_list*/
        if (ans_tmp.empty() == true)
        {
            command.error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss FCU/FCV code, flabel and lrm_list.")));
            return command;
        }

        /* 转换code 为数字 */
        fcu_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (fcu_num.flag == -1)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                      ("FCU/FCV code's format ERROR.")));
        }
        /* code 超限 */
        else if (fcu_num.num > static_cast<int>(DataLimint::MAXSYNCODE))
        {
            command.error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                   (int)(row_cnt),
                     ("FCU/FCV code exceed " + to_string(DataLimint::MAXSYNCODE) + ".")));
        }
        /* code 不能为0 */
        if (fcu_num.num == 0)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("FCU/FCV code can't be 0.")));

        }
        /* 同步代码不能重复 */

        else
        {
            /* 搜索同步代码数组 */
            for ( i = 0; i < command.s_code.size(); i++)
            {
                if (command.s_code[i].code == fcu_num.num)
                {
                    command.error = 1;
                    status.error++;

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                             (int)(status.line_num + 1),
                             (int)(row_cnt),
                               ("FCU/FCV code can't be equal to other syn_code( " + to_string(command.s_code[i].line) + ").")));
                }
            }
            /* 记录同步代码 */

            command_reg.code = fcu_num.num;
            command_reg.line = status.line_num + 1;
            command.s_code.push_back(command_reg);
        }

        /* code 建议为十进制 */
        if (fcu_num.flag == 16)
        {
            status.warning++;
            m_lstLst.push_back(GlobalFunc::WARNINGFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                              ("FCU/FCV code should be decimal.")));
        }
        /* 取flabel */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少flabel lrm_list*/
        if (ans_tmp.empty() == true)
        {
            command.error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                           (int)(status.line_num + 1),
                           (int)(row_cnt),
                             ("Miss FCU/FCV flabel and lrm_list.")));
            return command;
        }
         /* 搜索标号数组,检查是否有此标号 */
        i = 0;
        while (true)
        {
            if (label[i].name == ans_tmp)
            {
                /* 标号不在本段 */
                if (label[i].section != to_string(status.section))
                {
                    command.error = 1;
                    status.error++;

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                               (int)(status.line_num + 1),
                               (int)(row_cnt),
                               ("Label " + label[i].name + " is not in this section.")));
                }
                /* 本标号是子序列,提出告警 */
                if(label[i].sub == 1)
                {
                    status.warning++;
                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                             (int)(status.line_num + 1),
                             (int)(row_cnt),
                             ("FCU/FCV label " + ans_tmp + " is a subsequence.")));
                }
                break;
            }
            i++;
            if (i == label.size() )
            {
                command.error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                            ("Can't find label " + ans_tmp + " .")));

                break;
            }

        }
        /* 取lrm_list */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /* 缺少lrm_list */
        if (ans_tmp.empty() == true )
        {
            command.error = 1;
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                        (int)(status.line_num + 1),
                        (int)(row_cnt),
                        ("Miss FCU/FCV lrm_list.")));

            return command;
        }
        int lrm_num = 0;
        while (true)
        {
            lrm_num++;
            /* 超过4个lrm */
            if (lrm_num >static_cast<int>( DataLimint::MAXERULRMNUM))
            {
                command.error = 1;
                status.error++;


                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                            ("The FCU/FCV lrm number exceed" + to_string(DataLimint::MAXERULRMNUM) + ".")));

            }


            /* 转换lrm 为数字 */
            fcu_num = GlobalFunc::StringToInt(ans_tmp);
            /* 若不是数字 */
            if(fcu_num.flag == -1)
            {
                command.error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                            (int)(status.line_num + 1),
                            (int)(row_cnt),
                            ("TFCU/FCV lrm's format ERROR..")));
             }

             /* lrm 超限 */
            else if(fcu_num.num >static_cast<int>( DataLimint::MAXLRMNUMBER))
            {
                command.error = 1;
                status.error++;

                m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                          (int)(status.line_num + 1),
                          (int)(row_cnt),
                          ("FCU/FCV lrm exceed" + to_string(DataLimint::MAXLRMNUMBER) + ".")));

            }


            /* 取下一个lrm */
            ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
            row_cnt += (uint)label_length;
            /* 没有多余的字符串 */
            if (ans_tmp.empty() == true)
            {
                return command;
            }


        }




    }
}

uint GramCheck::FREEGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;


        String ans_tmp, ans_last_tmp;

        GET_NUM free_times;
        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取FREE */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取free times*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少free times*/
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss FREE times.")));
            return 1;
        }

        /* 转换free times 为数字 */
        free_times = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (free_times.flag == -1)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                     ("FREE times's format ERROR.")));
        }


        /*free times 超限 */
        else if (free_times.num > static_cast<int>(DataLimint::MAXFREETIMES))
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("Each FREE times can not exceed " + to_string(DataLimint::MAXFREETIMES) + " .")));

        }

        m_nSsyncMissTime += free_times.num;



        /* free times 建议为十进制 */
        if (free_times.flag == 16)
        {
            status.warning++;
            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
             (int)(status.line_num + 1),
             (int)(row_cnt),
               ("FREE times should be decimal.")));

        }

        /* 取free times 后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("FREE unexpect " + ans_tmp)));
        }

        return error;
    }
}

uint GramCheck::INTGram(COMPILE_STATUS &status, List &m_ListLst, String ans)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;

        String strInfo; //错误信息
        String ans_tmp, ans_last_tmp;

        GET_NUM int_num;
        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取INT */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /* 取intcode   */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少intcode和lrm_list */
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss INT intcode and lrm_list.")));
            return 1;
        }

        /* 转换intcode 为数字 */
        int_num = GlobalFunc::StringToInt(ans_tmp);
        /* 若不是数字 */
        if (int_num.flag == -1)
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                    (int)(status.line_num + 1),
                    (int)(row_cnt),
                     ("INT intcode's format ERROR.")));
        }


        /*code 超限 */
        else if (int_num.num > static_cast<int>(DataLimint::MAXINTCODE))
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("FREE times exceed " + to_string(DataLimint::MAXINTCODE) + " .")));

        }

        if (int_num.num < 15)
        {

            status.warning++;

            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
               (int)(status.line_num + 1),
               (int)(row_cnt),
                 ("INT intcode less than 15.")));

        }

        /* intcode 建议为十进制 */
        if (int_num.flag == 16)
        {
            status.warning++;
            m_ListLst.push_back(GlobalFunc::WARNINGFORMAT(
             (int)(status.line_num + 1),
             (int)(row_cnt),
               ("INT intcode should be decimal.")));

        }
        /* 取',' */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少lrm_list */
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("Miss INT lrm_list.")));
            return 1;
        }
        /* 若不是',' */
        if (ans_tmp != ",")
        {
            error = 1;
            status.error++;

            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("Miss INT ','.")));
        }
        else
        {
            /* 取lrm_list */
            ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
            row_cnt += (uint)label_length;

        }
        /* 缺少lrm */
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("Miss INT lrm_list.")));
            return 1;
        }
        int lrm_num = 0;
        while (true)
        {

            lrm_num++;
            /* 超过32个lrm */
            if(lrm_num >static_cast<int>(DataLimint::MAXINTLRMNUM))
            {
                error = 1;
                status.error++;


                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                  (int)(status.line_num + 1),
                  (int)(row_cnt),
                    ("The INT lrm number exceed " + to_string(DataLimint::MAXINTLRMNUM)+".")));

            }

            /* 转换lrm 为数字 */
            int_num = GlobalFunc::StringToInt(ans_tmp);
            /* 若不是数字 */
            if(int_num.flag == -1)
            {
                error = 1;
                status.error++;
                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                  (int)(status.line_num + 1),
                  (int)(row_cnt),
                    ("INT lrm's format ERROR.")));

            }
            /* lrm 超限 */
            else if(int_num.num >static_cast<int>( DataLimint::MAXLRMNUMBER))
            {
                error = 1;
                status.error++;

                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                        (int)(status.line_num + 1),
                        (int)(row_cnt),
                        ("The INT lrm number exceed " + to_string(DataLimint::MAXLRMNUMBER) + ".")));

            }
             /* lrm 建议为十进制 */
            if(int_num.flag == 16)
            {
                status.warning++;

                m_ListLst.push_back(GlobalFunc::ERRORFORMAT(
                 (int)(status.line_num + 1),
                 (int)(row_cnt),
                   ("INT lrm should be decimal.")));

            }

            /* 取下一个lrm */
            ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
            row_cnt += (uint)label_length;

            /* 没有多余的字符串 */
            if (ans_tmp.empty() == true)
            {
                return error;
            }

        }
    }
}

uint GramCheck::JUMPGram(COMPILE_STATUS &status, String ans, List_LABEL_TABLE label)
{

    {
        uint error = 0;
        uint row_cnt = 0;
        int label_length;

        String ans_tmp, ans_last_tmp;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取CALL/CALLI */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;
        /*  取label*/
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        /* 缺少label */
        if (ans_tmp.empty() == true)
        {

            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                     (int)(status.line_num + 1),
                     (int)(row_cnt),
                       ("Miss JUMP/JUMPI label.")));
            return 1;
        }
        /* 编号是关键字 */
        if(!(GlobalFunc::CmpKW(ans_tmp) == -1))
        {
            status.error++;

            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                   (int)(status.line_num + 1),
                   (int)(row_cnt),
                     ("JUMP/JUMPI label can't be a keyword.")));
            return 1;

        }
        /* 搜索标号数组,检查是否有此标号 */
        int i = 0;
        while (true)
        {
            if (label[i].name == ans_tmp)
            {
                /* 标号不在本段 */
                if(label[i].section != to_string(status.section))
                {
                    error = 1;
                    status.error++;

                    m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                   (int)(status.line_num + 1),
                                   (int)(row_cnt),
                                     ("JUMP/JUMPI label "+ label[i].name +" is not in this section or is same as other label name.")));

                 }
                 /* 本标号是子序列 */
                if(label[i].sub == 1)
                {
                    error = 1;
                    status.error++;

                      m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                   (int)(status.line_num + 1),
                                   (int)(row_cnt),
                                  ("JUMP/JUMPI label " + ans_tmp + "  can't be a subsequence.")));

                }
                break;

            }

            i++;
            if(i == label.size())
            {
                error = 1;
                status.error++;
               m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
                                   (int)(status.line_num + 1),
                                   (int)(row_cnt),
                                     ("Can't find JUMP/JUMPI label " + ans_tmp + " .")));
                break;
            }
        }

        /* 取label 后的字符串  */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        if (!ans_tmp.empty() == true)
        {
            error = 1;
            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("JUMP/JUMPI unexpect " + ans_tmp)));
        }

        return error;
    }
}

uint GramCheck::RETGram(COMPILE_STATUS &status, String ans)
{

    {

        uint row_cnt = 0;
        int label_length;

        String ans_tmp, ans_last_tmp;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取RET */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;


        /* 取RET 后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp,label_length);
        row_cnt += (uint)label_length;

        if (!ans_tmp.empty() == true)
        {

            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("RET/RETI unexpect " + ans_tmp)));

             return 1;
        }
        return 0;
    }
}

uint GramCheck::SSYNCGram(COMPILE_STATUS &status, String ans)
{

    {

        uint row_cnt = 0;
        int label_length;

        String ans_tmp, ans_last_tmp;

        /* 如果前边有标号,取标号 */
        if (status.label_flag == 1)
        {
            row_cnt = GlobalFunc::CutLabel( ans);
            status.label_flag = 0;
        }


        /* 取SSYNC */
        ans_tmp = GetString(ans, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;


        /* 取RET 后的字符串 */
        ans_tmp = GetString(ans_last_tmp, ans_last_tmp, label_length);
        row_cnt += (uint)label_length;

        if (!ans_tmp.empty() == true)
        {

            status.error++;
            m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
              (int)(status.line_num + 1),
              (int)(row_cnt),
                ("SSYNC unexpect " + ans_tmp)));

            return 1;
        }
        return 0;
    }
}

void GramCheck::LoadSrcFile()
{
    std::string strFile = m_strDir + "/" + m_strFlieName + ".src";
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

String GramCheck::GetString(String ans_sor, String &ans_left, int &length)
{
    ans_sor = trimStart(ans_sor);
    ans_left = ans_sor;

    /* 除了空格就是空字符串 */
    if (ans_left.empty())
    {
        length = 0;
        return ans_left;
    }

    int nIndex = ans_left.find_first_of(" ");
    string ans_tmp = ans_left.substr(0, nIndex);
    ans_left = ans_left.substr(nIndex + 1, ans_left.size() - nIndex - 1);
    length = ans_tmp.length();

    return ans_tmp;
}


