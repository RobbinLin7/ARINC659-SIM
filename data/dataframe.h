#ifndef DATAFRAME_H
#define DATAFRAME_H
#include<string>
typedef unsigned int uint;

class DataFrame
{
public:
    DataFrame();

private:
    uint framePeriod;
    std::string frameIdentification;
};

#endif // DATAFRAME_H
