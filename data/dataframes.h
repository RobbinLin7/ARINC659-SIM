#ifndef DATAFRAMES_H
#define DATAFRAMES_H

#include "data/dataframe.h"
#include <map>
#include <vector>


class DataFrames
{
public:
    class Iterator{
    public:
        Iterator(std::map<std::string, DataFrame>* dataFrames, std::vector<std::string>::iterator itr):
            dataFrames(dataFrames),itr(itr){}
        DataFrame& operator*();
        DataFrame* operator->();
        //const DataFrame* operator->()const;
        Iterator& operator++();
        Iterator& operator--();
//        const Iterator& operator++()const;
//        const Iterator& operator--()const;
        //bool operator==(const Iterator&)const;
        bool operator==(const Iterator&)const;
        bool operator==(const Iterator&);
        //bool operator!=(const Iterator&)const;
        bool operator!=(const Iterator&)const;
        bool operator!=(const Iterator&);
//        Iterator operator++(int);
//        Iterator operator--(int);
    private:
        std::map<std::string, DataFrame>* dataFrames;
        std::vector<std::string>::iterator itr;

    };
    class const_Iterator{
    public:
        const_Iterator(const std::map<std::string, DataFrame>* const_dataFrames, std::vector<std::string>::const_iterator const_itr):
            const_dataFrames(const_dataFrames), const_itr(const_itr){}
        const DataFrame& operator*();
        const DataFrame* operator->();
        const_Iterator& operator++();
        const_Iterator& operator--();
        bool operator==(const const_Iterator&)const;
        bool operator!=(const const_Iterator&)const;
        bool operator==(const const_Iterator&);
        bool operator!=(const const_Iterator&);

    private:
        const std::map<std::string, DataFrame>* const_dataFrames;
        std::vector<std::string>::const_iterator const_itr;
    };

    DataFrames();
    DataFrame& operator[](const std::string& identification);
    bool empty() const;
    Iterator begin();
    Iterator end();
    const_Iterator begin()const;
    const_Iterator end()const;
    const_Iterator cbegin() const;
    const_Iterator cend() const;
    Iterator find(const DataFrame& dataFrame);
    Iterator find(const std::string& identification);
    const_Iterator find(const DataFrame& dataFrame)const;
    const_Iterator find(const std::string& identification)const;
    DataFrame& at(const std::string& identification);
    void push(const DataFrame& dataFrame);
    void erase(const std::string& identification);
    void swap(int index1, int index2);
    size_t size() const;
private:
    std::map<std::string, DataFrame> dataFrames;   //帧数组
    std::vector<std::string> dataFramesOrder;      //帧数组顺序
    int index = 0;
};

#endif // DATAFRAMES_H
