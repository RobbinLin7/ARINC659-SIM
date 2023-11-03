#ifndef PREPROCESS_H
#define PREPROCESS_H

#include<string>
#include<list>
#include "tools/globalfunc.h"
#include "tools/user.h"
using std::list;
using std::string;
typedef unsigned int uint;
class PreProcess
{
typedef std::string String;

public:
    PreProcess();
    struct Macro_Format{
        String macro_name;  //宏定义名称
        String macro_context;   //宏定义内容
    };
//    struct COMPILE_STATUS
//    {
//        uint  line_num;      /* 行号 */
//        uint label_flag;    /* 标号标志 */
//        uint error;         /* 编译状态,报错 */
//        uint warning;       /* 编译状态,警告 */
//        int section;       /* 段编号 */
//    };

    PreProcess(String strDir,String strFileName){
       this->m_strDir = strDir;
       this->m_strFlieName = strFileName;
      // LoadSourceCommand();
   }
   int ProcessCommand(COMPILE_STATUS&  status);
   int MacroSubstitute(COMPILE_STATUS& status);
   int IsValidChar(char c);
   void SaveSrcFile();
   void SaveLstFile();
private:
    String m_strDir; //文件路径
    String m_strFlieName;//文件名称
    list<String> m_lstSrc; //预处理结果src命令列表
    list<String> m_lstLst; //处理结果lst 命令列表
    list<String> m_lstSourceCommand; //源文件列表
    list<String> m_lstCommand;
};

#endif // PREPROCESS_H

