#include "dataframes.h"
#include <iostream>
DataFrames::DataFrames()
{

}

bool DataFrames::empty() const
{
    return dataFramesOrder.empty();
}

DataFrames::Iterator DataFrames::begin()
{
    return empty() ? end() : Iterator(&dataFrames, dataFramesOrder.begin());
}

DataFrames::Iterator DataFrames::end()
{
    return Iterator(&dataFrames, dataFramesOrder.end());
}

DataFrames::const_Iterator DataFrames::begin() const
{
    return const_Iterator(&dataFrames, dataFramesOrder.cbegin());
}

DataFrames::const_Iterator DataFrames::end() const
{
    return const_Iterator(&dataFrames, dataFramesOrder.cend());
}

DataFrames::const_Iterator DataFrames::cbegin() const
{
    return const_Iterator(&dataFrames, dataFramesOrder.cbegin());
}

DataFrames::const_Iterator DataFrames::cend() const
{
    return const_Iterator(&dataFrames, dataFramesOrder.cend());
}

//DataFrames::Iterator DataFrames::begin() const
//{
//    std::cout << "in begin" << std::endl;
//    std::cout << (dataFramesOrder.begin() == dataFramesOrder.end()) << std::endl;
//    return empty() ? end() : Iterator(&dataFrames, dataFramesOrder.begin());
//}

//DataFrames::Iterator DataFrames::end() const
//{
//    std::cout << "in end" << std::endl;
//    std::cout << (dataFramesOrder.begin() == dataFramesOrder.end()) << std::endl;
//    return Iterator(&dataFrames, dataFramesOrder.end());
//}

//const DataFrames::Iterator DataFrames::cbegin() const
//{
//    return Iterator(&dataFrames, dataFramesOrder.begin());
//}

//const DataFrames::Iterator DataFrames::cend() const
//{
//    return Iterator(&dataFrames, dataFramesOrder.cend());
//}



DataFrames::Iterator DataFrames::find(const DataFrame &dataFrame)
{
    DataFrames::Iterator itr = this->begin();
    for(; itr != this->end(); ++itr){
        if(itr->getFrameIdentification() == dataFrame.getFrameIdentification()) break;
    }
    return itr;
}

DataFrames::Iterator DataFrames::find(const std::string &identification)
{
    DataFrames::Iterator itr = this->begin();
    for(; itr != this->end(); ++itr){
        if(itr->getFrameIdentification() == identification) break;
    }
    return itr;
}

DataFrames::const_Iterator DataFrames::find(const DataFrame &dataFrame)const
{
    DataFrames::const_Iterator itr = this->cbegin();
    for(; itr != this->cend(); ++itr){
        if(itr->getFrameIdentification() == dataFrame.getFrameIdentification()) break;
    }
    return itr;
}

DataFrames::const_Iterator DataFrames::find(const std::string &identification)const
{
    DataFrames::const_Iterator itr = this->cbegin();
    for(; itr != this->cend(); ++itr){
        if(itr->getFrameIdentification() == identification) break;
    }
    return itr;
}

DataFrame &DataFrames::at(const std::string &identification)
{
    return dataFrames.at(identification);
}

void DataFrames::push(const DataFrame &dataFrame)
{
    dataFrames.insert(std::make_pair(dataFrame.getFrameIdentification(), dataFrame));
    dataFramesOrder.push_back(dataFrame.getFrameIdentification());
}

void DataFrames::erase(const std::string &identification)
{
    if(this->find(identification) != this->end()){
        dataFrames.erase(identification);
        for(auto itr = dataFramesOrder.begin(); itr != dataFramesOrder.end(); ++itr){
            if(*itr == identification){
                dataFramesOrder.erase(itr);
                break;
            }
        }
    }
}

void DataFrames::swap(int index1, int index2)
{
    std::swap(dataFramesOrder.at(index1), dataFramesOrder.at(index2));
}

size_t DataFrames::size() const
{
    return dataFramesOrder.size();
}

DataFrame &DataFrames::Iterator::operator*(){
    return dataFrames->at(*itr);
}

DataFrame* DataFrames::Iterator::operator->()
{
    return &dataFrames->at(*itr);
}


//const DataFrame* DataFrames::Iterator::operator->() const
//{
//    //std::cout << *const_itr << std::endl;
//    return &const_dataFrames->at(*const_itr);
//}


DataFrame &DataFrames::operator[](const std::string& identification)
{
    return dataFrames.at(identification);
}



DataFrames::Iterator& DataFrames::Iterator::operator++()
{
    ++itr;
    return *this;
}

DataFrames::Iterator& DataFrames::Iterator::operator--()
{
    --itr;
    return *this;
}

//const DataFrames::Iterator& DataFrames::Iterator::operator++() const
//{
//    ++const_itr;
//    return *this;
//}

//const DataFrames::Iterator& DataFrames::Iterator::operator--() const
//{
//    --const_itr;
//    return *this;
//}




//bool DataFrames::Iterator::operator==(const Iterator& itr) const
//{
//    return (this->const_itr == itr.const_itr);
//}

bool DataFrames::Iterator::operator==(const Iterator& itr)const
{
    return (this->itr == itr.itr);
}

bool DataFrames::Iterator::operator!=(const Iterator& itr)const
{
    return (this->itr != itr.itr);
}

bool DataFrames::Iterator::operator==(const Iterator& itr)
{
    return (this->itr == itr.itr);
}

bool DataFrames::Iterator::operator!=(const Iterator& itr)
{
    return (this->itr != itr.itr);
}


//bool DataFrames::Iterator::operator!=(const Iterator& itr) const
//{
//    return (this->const_itr != itr.const_itr);
//}

const DataFrame &DataFrames::const_Iterator::operator*()
{
    return const_dataFrames->at(*const_itr);
}

const DataFrame *DataFrames::const_Iterator::operator->()
{
    return &const_dataFrames->at(*const_itr);
}

DataFrames::const_Iterator &DataFrames::const_Iterator::operator++()
{
    ++const_itr;
    return *this;
}

DataFrames::const_Iterator &DataFrames::const_Iterator::operator--()
{
    --const_itr;
    return *this;
}

bool DataFrames::const_Iterator::operator==(const const_Iterator &itr)const
{
    return (this->const_itr == itr.const_itr);
}

bool DataFrames::const_Iterator::operator!=(const const_Iterator &itr)const
{
    return (this->const_itr != itr.const_itr);
}

bool DataFrames::const_Iterator::operator==(const const_Iterator &itr)
{
    return (this->const_itr == itr.const_itr);
}

bool DataFrames::const_Iterator::operator!=(const const_Iterator &itr)
{
    return (this->const_itr != itr.const_itr);
}
