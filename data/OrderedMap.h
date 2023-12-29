#ifndef ORDEREDMAP_H
#define ORDEREDMAP_H
#include<map>
#include<vector>
template<typename K, typename V>
class OrderedMap
{
public:
    class Iterator{
    public:
        Iterator(std::map<K, V>* _map, typename std::vector<K>::iterator itr):
            _map(_map),_itr(itr){}
        V& operator*(){
            return _map->at(*_itr);
        }
        V* operator->(){
            return _map->at(*_itr);
        }
        const K& getKey(){
            return *_itr;
        }
        //const DataFrame* operator->()const;
        Iterator& operator++(){
            ++_itr;
            return *this;
        }
        Iterator& operator--(){
            --_itr;
            return *this;
        }
//        const Iterator& operator++()const;
//        const Iterator& operator--()const;
        //bool operator==(const Iterator&)const;
        bool operator==(const Iterator& iterator)const{
            return (this->_itr == iterator._itr);
        }
//        bool operator==(const Iterator& iterator){
//            return (this->_itr == iterator._itr);
//        }
        bool operator!=(const Iterator& iterator)const{
            return (this->_itr != iterator._itr);
        }
//        bool operator!=(const Iterator& iterator){
//            return (this->_itr != iterator._itr);
//        }
    private:
        std::map<K, V>* _map;
        typename std::vector<K>::iterator _itr;
    };
    class const_Iterator{
    public:
        const_Iterator(const std::map<K, V>* constMap, typename std::vector<K>::const_iterator const_iterator):
            constMap(constMap), const_iterator(const_iterator){}
        const V& operator*(){
            return constMap->at(*const_iterator);
        }
        const K& getKey(){
            return *const_iterator;
        }
        const V* operator->(){
            return &constMap->at(*const_iterator);
        }
        const_Iterator& operator++(){
            ++const_iterator;
            return *this;
        }
        const_Iterator& operator--(){
            --const_iterator;
            return *this;
        }
//        bool operator==(const const_Iterator& const_iterator)const{
//            return (this->const_iterator == const_iterator.const_iterator);
//        }
//        bool operator!=(const const_Iterator& const_iterator)const{
//            return (this->const_iterator != const_iterator.const_iterator);
//        }
        bool operator==(const const_Iterator& const_iterator){
            return (this->const_iterator == const_iterator.const_iterator);
        }
        bool operator!=(const const_Iterator& const_iterator){
            return (this->const_iterator != const_iterator.const_iterator);
        }
    private:
        const std::map<K, V>* constMap;
        typename std::vector<K>::const_iterator const_iterator;
    };

    OrderedMap(){}
    V& operator[](const K& key){
        return map[key];
    }
    bool empty() const{
        return map.empty();
    }
    Iterator begin(){
        return Iterator(&map, vector.begin());
    }
    Iterator end(){
        return Iterator(&map, vector.end());
    }
    const_Iterator begin()const{
        return const_Iterator(&map, vector.begin());
    }
    const_Iterator end()const{
        return const_Iterator(&map, vector.end());
    }
    const_Iterator cbegin() const{
        return const_Iterator(&map, vector.cbegin());
    }
    const_Iterator cend()const{
        return const_Iterator(&map, vector.cend());
    }
    bool exist(const K& key) const{
        if(map.find(key) != map.end()) return true;
        return false;
    }
    V& at(const K& key){
        return map[key];
    }
    void push(const K& key, const V& value){
        vector.push_back(key);
        map.insert(std::make_pair(key, value));
    }
    void erase(const K& key){
        map.erase(key);
        for(typename std::vector<K>::iterator itr = vector.begin(); itr != vector.end(); ++itr){
            if(*itr == key){
                vector.erase(itr);
                break;
            }
        }
    }
    void swap(int index1, int index2){
        std::swap(vector.at(index1), vector.at(index2));
    }
    size_t size() const{
        return vector.size();
    }
private:
    std::map<K, V> map;         //帧数组
    std::vector<K> vector;      //帧数组顺序
};
#include<iostream>
template<typename K,  typename V> bool operator==(const OrderedMap<K, V>& map1, const OrderedMap<K, V>& map2){
    if(map1.size() != map2.size()) return false;
    for(auto itr1 = map1.cbegin(), itr2 = map2.cbegin(); itr1 != map1.cend(); ++itr1, ++itr2){
        if(itr1.getKey() != itr2.getKey()){
            std::cout << "false here1\n";
            return false;
        }
        //由于没有实现!=符号，所以只能用==
        if((*itr1 == *itr2) == false){
            std::cout << "false here2\n";
            return false;
        }
    }
    return true;
}
#endif // ORDEREDMAP_H
