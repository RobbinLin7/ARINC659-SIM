#ifndef MODULE_H
#define MODULE_H
#include<string>
typedef unsigned int uint;
const uint minModuleNumber = 0, maxModuleNumber = 31;
class Module
{
public:
    Module();
    Module& operator=(const Module&);
    enum ModuleType{physicalModule, simulationModule};
    uint getModuleNumber() const;
    void setModuleNumber(uint newModuleNumber);

    uint getInitialWaitTime() const;
    void setInitialWaitTime(uint newInitialWaitTime);

    const std::string &getModuleName() const;
    void setModuleName(const std::string &newModuleName);


    ModuleType getModuleType() const;
    void setModuleType(ModuleType newModuleType);

private:
    uint moduleNumber;      //模块号
    uint initialWaitTime;   //初始等待时间
    std::string moduleName; //模块名称
    ModuleType moduleType;  //模块属性
};

#endif // MODULE_H
