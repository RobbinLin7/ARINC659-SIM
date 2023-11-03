#include "codegenerate.h"
#include <fstream>
#include <iostream>

uint CodeGenerate::GenerateCode(COMPILE_STATUS status, std::vector<LABEL_TABLE> label)
{
    //#region 变量定义及初始化
    uint error = 0;
    int line_num = 0;          /* 行号 */
    uint section_num = 0;       /* 段号 */
    int64_t mif_length = static_cast<int64_t>(CommandAddr::SECBASEADDR);    /* 全命令区长度,初值为参数区长度,即命令区首地址 */
    uint frame_length = 0;      /* 单一命令区长度 */
    int label_length = 0;
    String ans_tmp, ans_label;
    //#endregion
    /* mif文件初始化 */
    MifInit();
    LoadSrcFlie();


    m_lstLst.push_back("");
    m_lstLst.push_back("");
    ans_tmp = GlobalFunc::getDate();
    m_lstLst.push_back(ans_tmp);
    ans_tmp = "-- Generate Code.......";
    m_lstLst.push_back(ans_tmp);


    for (line_num = 0; (int)line_num < m_lstSrc.size(); line_num++)
    {
        //从第一行对每行分别生成代码

        ans_tmp = m_lstSrc[line_num];
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);

        if (ans_label.empty() == true)
        {
            continue;
        }
        /* 是END */
        if(ans_label == "END")
        {
            break;
        }


        /* 是SECTION */
        if(ans_label == "SECTION")
        {
            section_num = GlobalFunc::StringToInt(GlobalFunc::GetString(ans_tmp, ans_tmp, label_length)).num;

            /* 代码处理 */
            frame_length = (uint)FRAMEComp( (uint)(line_num + 1), label);

            /* 保存mif 文件*/
            SaveMifFile((int)section_num, (int)frame_length,mif_length);

            /* mif文件总长度计算 */
            mif_length += frame_length;
        }


    }

    /* mif文件超限判断 */
    // if(mif_length GREAT (pCommand->m_MaxBinLength))
    if (mif_length > 0XFFFF+1)
    {
        /* 记录文件越界 */
        std::cerr << "The length(" + to_string(mif_length)+ ") of file .mif exceed 0XFFFF ";
        //MessageBox.Show("The length("+to_string(mif_length)+") of file .mif exceed 0XFFFF " );

        error = 1;
        status.error++;



        m_lstLst.push_back(GlobalFunc::ERRORFORMAT(
               (int)(status.line_num + 1),
               0,
                 ("The length(" + to_string(mif_length) + ") of file .mif exceed  0XFFFF.")));

    }


    /* 打印文件总长度 */
    m_lstLst.push_back("The length of .mif is 0x" + to_string(mif_length) + ".");

    SaveLstFile();


    m_lstLst.clear();
    m_LstMif.clear();
    m_lstSrc.clear();





    return error;
}

void CodeGenerate::LoadSrcFlie()
{
    string strFile = m_strDir + "/" + m_strFlieName + ".src";
    if (strFile.length() == 0) return;
    m_lstSrc.clear();
    try
    {
        // Create an instance of StreamReader to read from a file.
        // The using statement also closes the StreamReader.
        std::ifstream is(strFile);
        if(is){
            string line;
            std::getline(is, line);
            m_lstSrc.push_back(line);
            is.close();
        }
    }
    catch (std::exception e)
    {
        // Let the user know what went wrong.
        std::cerr << e.what() << std::endl;
    }
}

void CodeGenerate::MifInit()
{

    string path = m_strDir + "/" + m_strFlieName + ".mif";

    try
    {
        std::ofstream os(path);
        if(os){
            /* 将所有空间初始化为32个0 */
            for(int i = 0; i < 65535; i++){
                os << "00000000000000000000000000000000" << std::endl;
            }
            os << "00000000000000000000000000000000";
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

int CodeGenerate::FRAMEComp(uint baseline, std::vector<LABEL_TABLE> label)

{


    uint have_gap_flag;         /* GAP命令标志 */
    uint have_delta_flag;       /* delta命令标志 */
    uint have_timescale_flag;   /* timescale命令标志 */
    int line_cnt =0;              /* .m文件的行号 */
    int label_length = 0;
    int line_num = 0;
    uint com_line_num;               /* 行号 */
    String ans_tmp, ans_label;
    uint nTmp;
    m_LstMif.clear();


    /* 将同步代码区首地址初始化为命令表首地址 */
    char* array_tmp1 = new char[33];
    BinaryNum(static_cast<uint>( CommandAddr::SGBTABLEADDR), array_tmp1);
    ans_tmp = string(array_tmp1);
    m_LstMif.push_back(ans_tmp);

    /* 将0X0001 ~ 0X010F的地址空间(同步代码区和参数区)初始化为32个0 */
    nTmp = static_cast<uint>(CommandAddr::SGBINITWAITADDR);
    for (line_num = 1; line_num < 0x0110; line_num++)
    {
        ans_tmp ="00000000000000000000000000000000";
        m_LstMif.push_back(ans_tmp);
    }
     /* 将0X0110 ~ 0X012F的地址空间(初始化延时时间)初始化为0XFFFFFFFF */
    nTmp = static_cast<uint>(CommandAddr::SGBRESVADDR);
    for (line_num = static_cast<int>(CommandAddr::SGBINITWAITADDR); line_num < nTmp; line_num++)
    {
        ans_tmp ="11111111111111111111111111111111";
        m_LstMif.push_back(ans_tmp);
    }

    /* 将0X0130 ~ 0X013F的地址空间(保留空间)初始化为32个0 */
    nTmp = static_cast<uint>(CommandAddr::SGBTABLEADDR);
    for (line_num = static_cast<int>(CommandAddr::SGBRESVADDR); line_num < nTmp; line_num++)
    {
        ans_tmp = "00000000000000000000000000000000";
        m_LstMif.push_back(ans_tmp);
    }
    have_gap_flag = 0;
    have_delta_flag = 0;
    have_timescale_flag = 0;
    line_cnt = 0;




    com_line_num = 1;
    line_cnt = (int)baseline;

    do
    {


        /* 读取到最后一行,退出 */
        if(line_cnt ==m_lstSrc.size())
        {
            break;
        }



        /* 取下一条指令 */
        ans_tmp = m_lstSrc[line_cnt];
        /* 取字串 */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);

            /* 不是关键字,是标号 */
        if(GlobalFunc::CmpKW(ans_label) == -1)
        {
            /* 取下一个字串 */
            ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
            /* 是SUB,则再取下一个字串 */
            if(ans_label =="SUB")
            {
                ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
            }
        }
        /* 是空行,取下一行 */
        if (ans_label.empty() == true)
        {
            continue;
        }

        /* ESECTION伪指令 */
        if(ans_label == "ESECTION")
        {
            ESECTIONComp();
            break;
        }

          /* TIMESCALE伪指令 */
        if(ans_label == "TIMESCALE")
        {
            have_timescale_flag = 1;
            TIMESCALEComp(ans_tmp,static_cast<uint>(CommandAddr::SGBSCALEADDR));
            continue;
        }

         /* GAP伪指令 */
        if(ans_label =="GAP")
        {
            have_gap_flag = 1;
            GAPComp( ans_tmp,static_cast<uint>(CommandAddr::SGBGAPADDR));
            continue;
        }
         /* DELTA伪指令 */
        if(ans_label =="DELTA")
        {
            have_delta_flag = 1;
            DELTAComp(ans_tmp, static_cast<uint>(CommandAddr::SGBDELTAADDR));
            continue;
        }
          /* VER伪指令 */
        if(ans_label == "VER")
        {
            VERComp( ans_tmp, static_cast<uint>(CommandAddr::SGBVERADDR));
            continue;
        }

       /* INITWAITTIME 伪指令 */
        if(ans_label =="INITWAITTIME")
        {
           INITWAITTIMEComp(ans_tmp, static_cast<int>(CommandAddr::SGBINITWAITADDR));
            continue;
        }
          /* BOW命令 */
        if(ans_label =="BOW")
        {
            line_cnt = BOWComp(ans_tmp, line_cnt);
            continue;
        }
        /* CALL命令 */
        if(ans_label =="CALL")
        {
            CALLComp( ans_tmp, label);
            continue;
        }
         /* CALLI命令 */
        if(ans_label == "CALLI")
        {
            CALLIComp( ans_tmp, label);
            continue;
        }
          /* ERU命令 */
        if(ans_label =="ERU")
        {
            uint eru_code;
            char* eru_array = new char[33];
            eru_code = ERUComp( ans_tmp);
            BinaryNum((uint)m_LstMif.size(), eru_array);
            m_LstMif[(int)eru_code] = String( eru_array);
            continue;
        }
         /* ERV命令 */
        if(ans_label =="ERV")
        {
            uint erv_code;
            char* erv_array = new char [33];
            erv_code = ERVComp( ans_tmp);
            BinaryNum((uint)m_LstMif.size(), erv_array);
            m_LstMif[(int)erv_code] = string(erv_array);
            continue;
        }
            /* FCU命令 */
        if(ans_label =="FCU")
        {
            FCUComp( ans_tmp, label);
            continue;
        }
          /* FCV命令 */
        if(ans_label =="FCV")
        {
            FCVComp(ans_tmp, label);
            continue;
        }
        /* FREE命令 */
        if(ans_label == "FREE")
        {
           FREEComp(ans_tmp);
            continue;
        }

           /* INT命令 */
        if(ans_label == "INT")
        {
           INTComp(ans_tmp);
           continue;
        }

         /* JUMP命令 */
        if(ans_label =="JUMP")
        {
            JUMPComp(ans_tmp, label);
            continue;
        }

         /* JUMPI命令 */
        if(ans_label =="JUMPI")
        {
            JUMPIComp(ans_tmp, label);
            continue;
        }

         /* RET命令 */
        if(ans_label =="RET")
        {
            RETComp();
            continue;
        }

         /* RETI命令 */
        if(ans_label =="RETI")
        {
            RETIComp();
            continue;
        }
         /* SSYNC命令 */
        if(ans_label == "SSYNC")
        {
            SSYNCComp();
            continue;
        }

    } while ((line_cnt++)>0);


    /* 若没有TIMESCALE命令,默认值为0 */
    if(have_timescale_flag == 0)
    {
        char* array_tmp = new char[33];
        BinaryNum(0, array_tmp);

        m_LstMif[static_cast<int>(CommandAddr::SGBSCALEADDR)] = string(array_tmp);
    }
     /* 若没有GAP命令,默认的消息时间间隔为2位长 */
    if(have_gap_flag == 0)
    {
        char* array_tmp = new char[33];
        BinaryNum(2, array_tmp);
        m_LstMif[static_cast<int>(CommandAddr::SGBGAPADDR)] = string(array_tmp);
    }

     /* 若没有DELTA命令,设置默认值为5 */
    if(have_delta_flag == 0)
    {
        char* array_tmp = new char[33];
        BinaryNum(5, array_tmp);

        m_LstMif[static_cast<int>(CommandAddr::SGBDELTAADDR)] = string(array_tmp);

    }


    line_cnt = m_LstMif.size();

    SaveMFile();
   // m_LstMif.clear();

    return line_cnt;


}
#include<cstring>
void CodeGenerate::BinaryNum(uint num, char num_array[])
{
    int index = 32;
    string strTmp = "00000000000000000000000000000000";
    //num_array = strTmp;
    strcpy(num_array, strTmp.c_str());


    while(--index >= 0)
    {

        uint ntest = num & (uint)(static_cast<uint>( 0X0001)<< index);
        if((num & (0X0001 << index))!=0)
        {
            num_array[31 - index] = '1';
        }
    }
    strTmp = string(num_array);
    return;
}

void CodeGenerate::ESECTIONComp()
{
    char* array_tmp = new char[33];
    unsigned short i;

    for(i = 0; i < 4; i++)
    {
        BinaryNum(static_cast<uint>(0xAAAAAAAA), array_tmp);
        m_LstMif.push_back(string(array_tmp));

    }
}

void CodeGenerate::TIMESCALEComp(String ans, uint addr)
{
    String ans_label, ans_tmp;
    int label_length;
    char* array_tmp = new char[33];

    /* 取time_scale_value */
    ans_label =GlobalFunc::GetString(ans, ans_tmp, label_length);
    /* 转换为数字字符串 */
    BinaryNum(GlobalFunc::StringToInt(ans_label).num, array_tmp);


    string str_tmp = string(array_tmp);
    m_LstMif[(int)addr] = string(array_tmp);

}

void CodeGenerate::GAPComp(String ans, uint addr)
{
    String ans_label, ans_tmp;
    int label_length;
    char* array_tmp = new char[33];

    /* 取bit_times*/
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);
    /* 转换为数字字符串 */



    BinaryNum(GlobalFunc::StringToInt(ans_label).num, array_tmp);

    m_LstMif[(int)addr] = string(array_tmp);

}

void CodeGenerate::DELTAComp(String ans, uint addr)
{
    String ans_label, ans_tmp;
    int label_length;
    char* array_tmp = new char[33];

    /* 取bit_times*/
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);
    /* 转换为数字字符串 */
    BinaryNum(GlobalFunc::StringToInt(ans_label).num, array_tmp);
    m_LstMif[(int)addr] = string(array_tmp);

}

void CodeGenerate::VERComp(String ans, uint addr)
{
    String ans_label, ans_tmp;
    int label_length;
    uint uint_tmp;
    char* array_tmp = new char[33];


    /* 取ver_value */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);
    /* 转换为数字字符串 */
    BinaryNum(GlobalFunc::StringToInt(ans_label).num, array_tmp);
    m_LstMif[(int)addr] = string(array_tmp);

    /* 取minor_ver */
    ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
    /* 转换为数字字符串 */
    uint_tmp = GlobalFunc::StringToInt(ans_label).num;

    /* 取cabinet_pos */
    ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);

     /* minor放在31~24, cabinet_pos放在23~0 */
    uint_tmp = (uint_tmp << 24) | (GlobalFunc::StringToInt(ans_label).num & 0X00FFFFFFFF);

    BinaryNum(uint_tmp, array_tmp);
    m_LstMif[(int)addr+1] = string(array_tmp);


}

void CodeGenerate::INITWAITTIMEComp(String ans, int addr)
{
    String ans_label, ans_tmp;
    int label_length;
    int m_num= 0;
    char* array_tmp = new char[33];
    ans_tmp = ans;
    while(true)
    {
        /* 取m_time */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
        /* m_time取完了或取够32个了 */
        if (ans_label.empty() == true | m_num > 32)
        {
            break;
        }

        /* 转换为数字字符串 */
        BinaryNum(GlobalFunc::StringToInt(ans_label).num, array_tmp);
        m_LstMif[(int)addr + m_num] = string(array_tmp); ;
        m_num++;
    }

}

int CodeGenerate::BOWComp(String ans, int line)
{
    String ans_label, ans_tmp;
    uint code[] = {0, 0, 0, 0};
    int label_length;
    uint tx_cnt, rx_cnt, tx_addr, rx_addr;
    uint uint_tmp;
    char* array_tmp = new char[33];
    tx_cnt = 0;
    rx_cnt = 0;
    tx_addr = 0;
    rx_addr = 0;
      /* 取size */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);
    uint_tmp = GlobalFunc::StringToInt(ans_label).num & 0X000000FF;
    uint_tmp--;

    code[0] = uint_tmp << 8;
    /* 进行TX/RX指令编译 */
    while ((line++)>0)
    {

        /* 取下一行 */
        ans_tmp = m_lstSrc[line];
        /* 取字符串 */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
        /* 是空行 */
        if (ans_label.empty() == true)
        {
            continue;
        }
         /* 若为RX命令 */
        if(ans_label =="RX")
        {
            rx_cnt++;
            /* 取RX的lrm号 */
            ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
            uint_tmp = GlobalFunc::StringToInt(ans_label).num;
            code[3] |=(uint) (static_cast<int>( 0X00000001) <<(int)uint_tmp);
            /* 取RX的地址 */
            ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
            rx_addr = GlobalFunc::StringToInt(ans_label).num;
            continue;
        }
         /* 若为TX命令 */
        if(ans_label =="TX")
        {
            tx_cnt++;
            /* 清lrm相应编号的数据 */
            code[2] &= ~(uint)((0X000000FF) << (int)(tx_cnt*8 - 8));
            /* 设置TX的LRM有效标志位 */
            code[2] |= (uint)(0X00000080 << (int)(tx_cnt*8 - 8));
            /* 取TX的lrm号 */
            ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
            uint_tmp = GlobalFunc::StringToInt(ans_label).num;
            code[2] |= (uint_tmp & 0X0000001F) << (int)(tx_cnt*8 - 8);
            /* 取TX的地址 */
            ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
            if(ans_label =="VERSION")
            {
                tx_addr =static_cast<uint>( CommandAddr::VERSIONADDR);
            }
            else
            {
                tx_addr = GlobalFunc::StringToInt(ans_label).num;
            }
            continue;
        }
        /* 非"TX"非"RX"非空行,标识BOW/TX/RX指令结束,文件重定位到读取字串之前的位置 */
        line--;
        break;

    }


    /* 只有一个TX,则机器码为0001,若大于一个TX,则机器码为1001 */
    if(tx_cnt > 1)
    {
        code[0] |= 0X00000009;
    }
    else
    {
        code[0] |= 0X00000001;
    }


    /* 清TX地址 */
    code[0] &= 0X0000FFFF;
    code[1] &= 0XFFFFFF00;
    /* 置TX地址 */
    code[0] |= (tx_addr & 0X0000FFFF) << 16;
    code[1] |= (tx_addr & 0X00FF0000) >> 16;

     /* 清RX地址 */
    code[1] &= 0X000000FF;
    /* 置RX地址 */
    code[1] |= (rx_addr & 0X00FFFFFF) << 8;
     /* 将机器码写入文件 */
    for(uint_tmp = 0; uint_tmp <= 3; uint_tmp++)
    {
        /* 转换为数字字符串 */
        int strTmp = (int)code[uint_tmp];
        BinaryNum(code[uint_tmp], array_tmp);
        m_LstMif.push_back(string(array_tmp));

    }
    return line;


}

void CodeGenerate::CALLComp(String ans, std::vector<LABEL_TABLE> label)
{
    String ans_label, ans_tmp;
    int label_length;
    uint uint_tmp = 0;
    char* array_tmp = new char[33];


    /* 取slabel */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);

    uint_tmp = 0;
    while (true)
    {
        if(ans_label == label[(int)uint_tmp].name)
        {
            /* uint_tmp = 0X00000006 BWOR ((label[uint_tmp].addr BWAND 0X00FFFFFF) << 8); */
            /* 地址只用16位,高8位填0 */
            uint_tmp =(uint) (0X00000006 | ((label[(int)uint_tmp].addr & 0X0000FFFF) << 8));
            break;
        }
        uint_tmp++;
    }
    /* 转换为数字字符串 */
    BinaryNum(uint_tmp, array_tmp);
    m_LstMif.push_back( string(array_tmp));

}

void CodeGenerate::CALLIComp(String ans, std::vector<LABEL_TABLE> label)
{
    String ans_label, ans_tmp;
    int label_length;
    uint uint_tmp = 0;
    char* array_tmp = new char[33];


    /* 取slabel */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);

    uint_tmp = 0;
    while (true)
    {
        if (ans_label == label[(int)uint_tmp].name)
        {
            /* uint_tmp = 0X00000006 BWOR ((label[uint_tmp].addr BWAND 0X00FFFFFF) << 8); */
            /* 地址只用16位,高8位填0 */
            uint_tmp = (uint)(0X0000000E | ((label[(int)uint_tmp].addr & 0X0000FFFF) << 8));
            break;
        }
        uint_tmp++;
    }
    /* 转换为数字字符串 */
    BinaryNum(uint_tmp, array_tmp);
    m_LstMif.push_back(string(array_tmp));

}

uint CodeGenerate::ERUComp(String ans)

{
    String ans_label, ans_tmp;

    uint code[] = {0, 0};
    uint code_num = 0;
    uint dot_flag = 0;
    int label_length;
    uint lrm_num = 0;
    uint uint_tmp;
    char* array_tmp = new char[33];

    ans_tmp = ans;
    while (true)
    {
        /* 取code或lrm_list */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
        if (ans_label.empty() == true)
        {
            break;
        }
        if(ans_label == ",")
        {
            dot_flag++;
            continue;
        }
        /* 在逗号之前,为code */
        if(dot_flag == 0)
        {
            /* 转换为数字字符串 */
            code_num = GlobalFunc::StringToInt(ans_label).num & 0X000000FF;
            code[0] = (uint)(code_num << 8) | 0X00000003;
        }
        else
        {
            lrm_num++;
            /* 清LRM相应编号的内容 */
            code[1] &= (uint)~((0X000000FF) << (8 * (int)lrm_num - 8));
            /* 设置LRM有效标志位 */
            code[1] |= (uint)0X00000080 << (8 * (int)lrm_num - 8);
            /* 取LRM编号 */
            code[1] |= (GlobalFunc::StringToInt(ans_label).num & 0X0000001F) << (8 * (int)lrm_num - 8);
        }

    }

    for(uint_tmp = 0; uint_tmp <= 1; uint_tmp++)
    {
        BinaryNum(code[(int)uint_tmp], array_tmp);
        m_LstMif.push_back(string(array_tmp));
    }
    return code_num;




}

uint CodeGenerate::ERVComp(String ans)

{
    String ans_label, ans_tmp;

    uint code[] = { 0, 0 };
    uint code_num = 0;
    uint dot_flag = 0;
    int label_length;
    uint lrm_num = 0;
    uint uint_tmp;
    char* array_tmp = new char[33];

    ans_tmp = ans;
    while (true)
    {
        /* 取code或lrm_list */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);
        if (ans_label.empty() == true)
        {
            break;
        }
        if (ans_label == ",")
        {
            dot_flag++;
            continue;
        }
        /* 在逗号之前,为code */
        if (dot_flag == 0)
        {
            /* 转换为数字字符串 */
            code_num = GlobalFunc::StringToInt(ans_label).num & 0X000000FF;
            code[0] = (uint)(code_num << 8) | 0X0000000B;
        }
        else
        {
            lrm_num++;
            /* 清LRM相应编号的内容 */
            code[1] &= (uint)~((0X000000FF) << (8 * (int)lrm_num - 8));
            /* 设置LRM有效标志位 */
            code[1] |= (uint)0X00000080 << (8 * (int)lrm_num - 8);
            /* 取LRM编号 */
            code[1] |= (GlobalFunc::StringToInt(ans_label).num & 0X0000001F) << (8 * (int)lrm_num - 8);
        }

    }

    for (uint_tmp = 0; uint_tmp <= 1; uint_tmp++)
    {
        BinaryNum(code[(int)uint_tmp], array_tmp);
        m_LstMif.push_back(string(array_tmp));
    }
    return code_num;




}

void CodeGenerate::FCUComp(String ans, std::vector<LABEL_TABLE> label)

{
    String ans_label, ans_tmp;

    uint code[] = {0, 0};
    uint code_num = 0;
    int label_length;
    uint label_flag = 0;
    uint lrm_num = 0;
    uint uint_tmp;
    char* array_tmp = new char[33];

    /* 取同步代码 */
    ans_label = GlobalFunc::GetString(ans,ans_tmp,label_length);
    code_num = GlobalFunc::StringToInt(ans_label).num & 0X000000FF;

    while (true)
    {

        /* 取flabel或lrm_list */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);

        if (ans_label.empty() == true )
        {
            break;
        }
        /* 取code */
        if(label_flag == 0)
        {
            label_flag++;
            /* 查找标号取得地址 */
            uint_tmp = 0;
            while(true)
            {
                /* 搜索标号,取其地址 */
                if(ans_label == label[(int)uint_tmp].name)
                {
                    code[0] = (uint)(0X00000004 | (code_num << 8) | ((label[(int)uint_tmp].addr & 0X0000FFFF) << 16));
                    /* 写同步代码表 */
                     BinaryNum((uint)label[(int)uint_tmp].addr,array_tmp);
                     m_LstMif[(int)code_num] = string(array_tmp);
                    break;

                }
                uint_tmp++;
            }
        }
        else
        {
            lrm_num++;
            /* 清LRM内容 */
            code[1] &= (uint)~((0X000000FF) << (8*(int)lrm_num - 8));
            /* 设置LRM相应编号的有效标志位 */
            code[1] |= (uint)(0X00000080) << (8 *(int) lrm_num - 8);
            /* 取LRM编号 */
            code[1] |= (GlobalFunc::StringToInt(ans_label).num & 0X0000001F) << (8 * (int)lrm_num - 8);
        }

    }

    for(uint_tmp = 0; uint_tmp <= 1; uint_tmp++)
    {

        BinaryNum(code[uint_tmp], array_tmp);
        m_LstMif.push_back(string(array_tmp));
    }


}

void CodeGenerate::FCVComp(String ans, std::vector<LABEL_TABLE> label)

{
    String ans_label, ans_tmp;

    uint code[] = { 0, 0 };
    uint code_num = 0;
    int label_length;
    uint label_flag = 0;
    uint lrm_num = 0;
    uint uint_tmp;
    char* array_tmp = new char[33];

    /* 取同步代码 */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);
    code_num = GlobalFunc::StringToInt(ans_label).num & 0X000000FF;

    while (true)
    {

        /* 取flabel或lrm_list */
        ans_label = GlobalFunc::GetString(ans_tmp, ans_tmp, label_length);

        if (ans_label.empty() == true)
        {
            break;
        }
        /* 取code */
        if (label_flag == 0)
        {
            label_flag++;
            /* 查找标号取得地址 */
            uint_tmp = 0;
            while (true)
            {
                /* 搜索标号,取其地址 */
                if (ans_label == label[(int)uint_tmp].name)
                {
                    code[0] = (uint)(0X0000000C | (code_num << 8) | ((label[(int)uint_tmp].addr & 0X0000FFFF) << 16));
                    /* 写同步代码表 */
                    BinaryNum((uint)label[(int)uint_tmp].addr, array_tmp);
                    m_LstMif[(int)code_num] = string(array_tmp);
                    break;

                }
                uint_tmp++;
            }
        }
        else
        {
            lrm_num++;
            /* 清LRM内容 */
            code[1] &= (uint)~((0X000000FF) << (8 * (int)lrm_num - 8));
            /* 设置LRM相应编号的有效标志位 */
            code[1] |= (uint)(0X00000080) << (8 * (int)lrm_num - 8);
            /* 取LRM编号 */
            code[1] |= (GlobalFunc::StringToInt(ans_label).num & 0X0000001F) << (8 * (int)lrm_num - 8);
        }

    }

    for (uint_tmp = 0; uint_tmp <= 1; uint_tmp++)
    {

        BinaryNum(code[uint_tmp], array_tmp);
        m_LstMif.push_back(string(array_tmp));
    }


}

void CodeGenerate::FREEComp(String ans)

{
    String ans_label, ans_tmp;
    int uint_tmp;
    char* array_tmp = new char[33];

    /* 取bit_times */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, uint_tmp);
     /* 转换为数字字符串并移位 */
    uint_tmp = (GlobalFunc::StringToInt(ans_label).num & 0X0000FFFF) << 8;
    BinaryNum(uint_tmp, array_tmp);

    m_LstMif.push_back(string(array_tmp));

}

void CodeGenerate::INTComp(String ans)

{
    String ans_label, ans_tmp;

    uint code[] = { 0, 0 };
    int label_length;
    char* array_tmp = new char[33];


    /* 取int_code */
    ans_label = GlobalFunc::GetString(ans, ans_tmp,label_length);
    /* 转换为数字字符串并移位 */
    code[0] = ((GlobalFunc::StringToInt(ans_label).num & 0X00FFFFFF) << 8) | 0X08;

    while (true)
    {
        /* 取lrm_list */
        ans_label = GlobalFunc::GetString(ans_tmp,  ans_tmp, label_length);
        if (ans_label.empty())
        {
            break;
        }
        /* 取到非逗号内容,即lrm号 */
        if(ans_label != ",")
        {
            code[1] = code[1] | (uint)(0X01 <<(int)GlobalFunc::StringToInt(ans_label).num);

        }
    }
    /* 借用label_length */
    for(label_length = 0; label_length <= 1; label_length++)
    {
        BinaryNum(code[(int)label_length], array_tmp);
        m_LstMif.push_back(string(array_tmp));
    }

}

void CodeGenerate::JUMPComp(String ans, std::vector<LABEL_TABLE> label)

{
    String ans_label, ans_tmp;
    int label_length;
    int uint_tmp;
    char* array_tmp = new char[33];

    /* 取label */
    ans_label =GlobalFunc::GetString(ans, ans_tmp, label_length);

    uint_tmp = 0;
    while (true)
    {
         /* 搜索标号,取其地址 */
        if(ans_label == label[uint_tmp].name)
        {
            /* uint_tmp = 0X00000005 BWOR ((label[uint_tmp].addr BWAND 0X00FFFFFF) << 8); */
            /* 地址只用16位,高8位填0 */
            uint_tmp = 0X00000005 | ((label[uint_tmp].addr & 0X0000FFFF) << 8);
            break;
        }
        uint_tmp++;

    }
    /* 转换为数字字符串*/
    BinaryNum((uint)uint_tmp, array_tmp);
    m_LstMif.push_back(string(array_tmp));


}

void CodeGenerate::JUMPIComp(String ans, std::vector<LABEL_TABLE> label)

{
    String ans_label, ans_tmp;
    int label_length;
    int uint_tmp;
    char* array_tmp = new char[33];

    /* 取label */
    ans_label = GlobalFunc::GetString(ans, ans_tmp, label_length);

    uint_tmp = 0;
    while (true)
    {
        /* 搜索标号,取其地址 */
        if (ans_label == label[uint_tmp].name)
        {
            /* uint_tmp = 0X00000005 BWOR ((label[uint_tmp].addr BWAND 0X00FFFFFF) << 8); */
            /* 地址只用16位,高8位填0 */
            uint_tmp = 0X0000000D | ((label[uint_tmp].addr & 0X0000FFFF) << 8);
            break;
        }
        uint_tmp++;
    }
    /* 转换为数字字符串*/
    BinaryNum((uint)uint_tmp, array_tmp);
    m_LstMif.push_back(string(array_tmp));


}
void CodeGenerate::RETComp()

{

     char* array_tmp = new char[33];

     BinaryNum(0X0007, array_tmp);
     m_LstMif.push_back(string(array_tmp));


}

void CodeGenerate::RETIComp()

{

    char* array_tmp = new char[33];

    BinaryNum(0X000F, array_tmp);
    m_LstMif.push_back(string(array_tmp));
}

void CodeGenerate::SSYNCComp()

{

    char* array_tmp = new char[33];

    BinaryNum(0X0002, array_tmp);
    m_LstMif.push_back(string(array_tmp));
}

void CodeGenerate::SaveMFile()

{


    string path = m_strDir + "/" + m_strFlieName + ".m";


    try
    {
        std::ofstream os(m_strDir);
        if(os){
            for (int i = 0; i < m_LstMif.size(); i++){
                os << m_LstMif[i] << std::endl;
            }
            os.close();
        }

    }
    catch (std::exception e)
    {
        // Let the user know what went wrong.
        std::cerr << e.what() << std::endl;
    }
}

void CodeGenerate::SaveMifFile(int section_num, int frame_length, int64_t mif_length)
{
    string path = m_strDir + "/" + m_strFlieName + ".mif";

    std::fstream fs(path);
    if(fs){
        char* array_tmp = new char[33];
        BinaryNum(((uint)mif_length), array_tmp);     /* 当前地址为实际长度-1,所以当前地址的下一个地址为实际长度 */
        /* 修改地址分配表 */

       int n = (int) (static_cast<int>(CommandAddr::TABLEADDR) + section_num);
       fs.seekp(34 * static_cast<int>(CommandAddr::TABLEADDR), std::ios::beg);
       String str_tmp = string(array_tmp);
       int NLent = str_tmp.length();
       str_tmp += "\r\n";
       fs.write(str_tmp.c_str(), str_tmp.size());
       fs.seekp(34 * mif_length, std::ios::beg);
       for( int uint_tmp = 0; uint_tmp < frame_length; uint_tmp++)
       {
           str_tmp = m_LstMif[uint_tmp]+"\r\n";
           fs.write(str_tmp.c_str(), str_tmp.size());
           fs.flush();
       }
       //ASCIIEncoding uniEncoding = new ASCIIEncoding();
       //fileStream.Seek(34*(static_cast<int>( CommandAddr::TABLEADDR) + section_num),SeekOrigin.Begin);
    }
    mif_length += frame_length;
    try
    {
        fs.close();
    }catch(std::exception ex){

    }
    m_LstMif.clear();
}

void CodeGenerate::SaveLstFile()
{

    string path = m_strDir + "/" + m_strFlieName + ".lst";

    try
    {
        std::ofstream os(path);
        if(os){
            for(int i = 0; i < m_lstSrc.size(); i++){
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
