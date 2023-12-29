#include "module.h"

Module::Module()
{

}

//Module &Module::operator=(const Module &module)
//{
//    this->moduleName = module.getModuleName();
//    this->moduleType = module.getModuleType();
//    this->moduleNumber = module.getModuleNumber();
//    this->initialWaitTime = module.getInitialWaitTime();
//    return *this;
//}

uint Module::getModuleNumber() const
{
    return moduleNumber;
}

void Module::setModuleNumber(uint newModuleNumber)
{
    moduleNumber = newModuleNumber;
}

uint Module::getInitialWaitTime() const
{
    return initialWaitTime;
}

void Module::setInitialWaitTime(uint newInitialWaitTime)
{
    initialWaitTime = newInitialWaitTime;
}

const std::string &Module::getModuleName() const
{
    return moduleName;
}

void Module::setModuleName(const std::string &newModuleName)
{
    moduleName = newModuleName;
}

Module::ModuleType Module::getModuleType() const
{
    return moduleType;
}

void Module::setModuleType(ModuleType newModuleType)
{
    moduleType = newModuleType;
}

bool operator==(const Module& module1, const Module& module2){
    return (module1.getInitialWaitTime() == module2.getInitialWaitTime()) &&
           (module1.getModuleName() == module2.getModuleName()) &&
           (module1.getModuleNumber() == module2.getModuleNumber()) &&
           (module1.getModuleType() == module2.getModuleType());
}

