#include "preprocess.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include "tools/globalfunc.h"
#include "tools/user.h"
using std::to_string;
std::string trim(std::string s);
std::string trimStart(std::string s);
string toUpper(string s){
    for(size_t i = 0; i < s.length(); i++){
        if(s[i] >= 'a' && s[i] <= 'z') s[i] = 'A' + s[i] - 'a';
    }
    return s;
}
std::string ReplaceString(std::string original, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = original.find(from, start_pos)) != std::string::npos) {
        original.replace(start_pos, from.length(), to);
        start_pos += to.length(); // 在替换的子串之后继续查找
    }
    return original;
}
PreProcess::PreProcess()
{

}
int PreProcess::ProcessCommand(COMPILE_STATUS &status)
{
    int error = 0;
     int line_cnt = 0;
     string ans_tmp;
     string ans_print;
     int temp;

     //m_lstCommand = new List<String>();
     m_lstCommand.clear();
     for(auto command : m_lstSourceCommand){
        m_lstCommand.push_back(command);
     }

     /* 记录预处理时间 */
     auto currentTime = std::chrono::system_clock::now();
     std::time_t current_time_t = std::chrono::system_clock::to_time_t(currentTime);
     ans_tmp = std::ctime(&current_time_t);
     m_lstLst.push_back(ans_tmp);

     ans_tmp = "-- PreProcess......";
     m_lstLst.push_back(ans_tmp);


     if (m_lstCommand.size() == 0)
     {

         return  1 ;
     }

     if(MacroSubstitute(status)>0)
     {
         return 1;
         goto Finish;

     }

     for(auto ans_tmp : m_lstCommand){
         if(ans_tmp == ""){
             m_lstSrc.push_back("");
             continue;
         }
         int blank_flag = 0;
         temp = 0; //在VC字符串数组下标从0开始
         while(true)
         {

             char ste= ans_tmp[temp];
             /* 遇到';',本行结束 */
             if(ans_tmp[temp] == ';')
             {
                 ans_tmp=  ans_tmp.insert(temp," ");
                 temp++;
                 break;
             }

             /* 全空格判断,若blank_flag为0,表示该行全为0 */
             if((ans_tmp[temp] != ' ') & (ans_tmp[temp] != '\t'))//'\t'水平制表（跳到下一个TAB位置）
             {
                 blank_flag = 1;
             }
             /* 无效字符 */
             if (IsValidChar(ans_tmp[temp])==0)
             {

                 int nLine = line_cnt + 1;
                 char c = ans_tmp[temp];

                 ans_print = "[ERROR] Line " + to_string(nLine) + " ( " + to_string(temp) + " row ):Invalid char" + to_string(c) + ".";

                 m_lstLst.push_back(ans_print);
                 status.error++;
                 error = 1;
             }

             /* ','前后加空格 */
             if(ans_tmp[temp] == ',')
             {
                 ans_tmp=  ans_tmp.insert(temp + 1," ");
                 ans_tmp= ans_tmp.insert(temp," ");
                 temp += 2;
             }

             temp++;
             /* 若读到行尾还没有遇到';'且该行不是空格行 */
             if(temp == ans_tmp.length())
             {
                 if(blank_flag==1)
                 {
                     /* 避免一条指令后没有空格直接换行 */
                     ans_tmp= ans_tmp.insert(temp," ");
                     temp++;
                     int nLine = line_cnt + 1;

                     ans_print = "[Warning] Line " + to_string(nLine) + " ( " + to_string(temp) + " row ):Miss ';'.";

                     m_lstLst.push_back(ans_print);


                    status.warning++;
                 }
                 break;
             }
         }
         ans_tmp = ans_tmp.substr(0, temp);
         //ans_tmp = ans_tmp.substr(0,temp);
         m_lstSrc.push_back(ans_tmp);
     }
  Finish:
      SaveSrcFile();
      SaveLstFile();
      m_lstLst.clear();
     // m_lstSourceCommand.Clear();
      m_lstSrc.clear();
      return error;

}

int PreProcess::MacroSubstitute(COMPILE_STATUS &status)
{
    int line_cnt = 0;
    string str_tmp, str_line,str_keyword;
    int nTmp = 0;

    std::vector<Macro_Format> macroList;


//#region 逐行查找宏定义

    for (line_cnt = 0; line_cnt < m_lstCommand.size(); line_cnt++)
    {

        Macro_Format macro;
        str_line = m_lstCommand[line_cnt];
        str_line = trim(str_line);

        if (str_line.size() < 8) continue;/*less than 8 ; not enough for a macro #define */

        str_tmp = str_line.substr(0, 8);
        if (str_tmp != "#define ") continue;//current line is not a #define  macro

        str_line = str_line.substr(8, str_line.size() - 8);//remove the key word #define
        str_line = trim(str_line);

        nTmp = str_line.find_first_of(' ');
        str_tmp= str_line.substr(0, nTmp);/*first word is  macro name */

        str_keyword = trim(str_tmp);
        transform(str_keyword.begin(),str_keyword.end(),str_tmp.begin(),::tolower);
        //str_tmp = str_keyword.ToUpper();
        int int_tmp = GlobalFunc::CmpKW(str_tmp);

        if (int_tmp != -1)
        {
            str_tmp = "[ERROR] Line " + to_string(line_cnt + 1) + " macro conflict with key words:." + str_tmp;

            m_lstLst.push_back(str_tmp);
            status.error++;
            return 1;

        }

        for (int i = 0; i < macroList.size(); i++ )
        {
            if (str_tmp == macroList[i].macro_name)
            {

                str_tmp = "[ERROR] Line  " + to_string(line_cnt + 1) + "  macro redefinition :" + str_tmp;
                m_lstLst.push_back(str_tmp);
                status.error++;
            }

        }


        macro.macro_name = str_keyword;

        str_line = str_line.substr(nTmp, str_line.size() - nTmp);/*the last string is actual value*/
        str_line = trim(str_line);

        nTmp = str_line.find_first_of(' ');
        if (nTmp!=-1)
        {
            str_tmp = "[ERROR] Line " + to_string(line_cnt + 1) + " macro #define  is invalid .";

            m_lstLst.push_back(str_tmp);
            status.error++;
            return 1;
        }
        macro.macro_context = str_line;
        m_lstCommand[line_cnt] = "";

        macroList.push_back(macro);

    }


//#endregion


//#region 宏替换

    for (line_cnt = 0; line_cnt < m_lstSourceCommand.size(); line_cnt++)
    {

        str_line = m_lstCommand[line_cnt];

        str_line = trim(str_line);
        int ptr;
        for(int i =0 ; i<macroList.size(); i++)
        {
            str_tmp = trim(macroList[i].macro_name);
            str_tmp = " " + str_tmp + " ";
            //m_lstCommand[line_cnt] = m_lstCommand[line_cnt].Replace(str_tmp, " " + macroList[i].macro_context+ " ");
            if((ptr = m_lstCommand[line_cnt].find(str_tmp)) != string::npos){
                m_lstCommand[line_cnt].replace(ptr, str_tmp.size(), " " + macroList[i].macro_context+ " ");
            }

        }


    }


//#endregion


    return 0;
}

int PreProcess::IsValidChar(char c)
{
    if((c == ',') || (c == ';') || (c == '_')
        || (c == ' ') || (c == '\t')
        || (c == '\n')
        || ((c <= 'Z') && (c >= 'A'))
        || ((c <= 'z') && (c >= 'a'))
        || ((c <= '9') && (c >= '0')))

    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void PreProcess::SaveSrcFile()
{
    string path = m_strDir + "/" + m_strFlieName + ".src";
    try{
        std::ofstream os(path);
        for(int i = 0; i < m_lstSrc.size(); i++){
            os << m_lstSrc[i] << std::endl;
        }
        os.close();
    }catch(std::exception e){
        std::cerr << e.what();
    }

//    try
//    {

//        using (StreamWriter sw = File.CreateText(path))
//        {

//            for (int i = 0; i < m_lstSrc.Count; i++)
//            {
//                sw.WriteLine(m_lstSrc[i]);
//            }

//            sw.Close();
//        }

//    }
//    catch (Exception e)
//    {
//        // Let the user know what went wrong.
//    }
}

void PreProcess::SaveLstFile()
{

}

void PreProcess::setLstSourceCommand(const vector<String> &newLstSourceCommand)
{
    m_lstSourceCommand = newLstSourceCommand;
}


