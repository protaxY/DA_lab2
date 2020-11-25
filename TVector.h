#ifndef LAB1_TVector_H
#define LAB1_TVector_H
#include <iostream>
template <class T>
class TVector{
private:
    unsigned long long TVectorSize;
    unsigned long long TVectorCapacity;
    T* Data;
public:
    TVector();
    unsigned long long Size();
    void PushBack(const T elem);
    void PopBack();
    void Insert(const unsigned long long &pos,const T &elem);
    void OrdinaryInsert (const T &elem);
    void OrdinaryErase (const T &elem);
    void Erase(const unsigned long long &pos);
    T& operator[] (long long iterator);
    ~TVector();
};

template <class T>
TVector<T>::TVector(){
    TVectorSize = 0;
    TVectorCapacity = 0;
    Data = nullptr;
}
template <class T>
unsigned long long TVector<T>::Size(){
    return TVectorSize;
}
template <class T>
void TVector<T>::PushBack(const T elem){
    if (TVectorCapacity == 0){
        TVectorCapacity = 1;
        TVectorSize = 0;
        Data = new T[TVectorCapacity];
    }
    else if (TVectorCapacity == TVectorSize){
        TVectorCapacity *= 2;
        T* newData = new T[TVectorCapacity];
        for (unsigned long long i = 0; i < TVectorSize; ++i){
            newData[i] = Data[i];
        }
        delete [] Data;
        Data = newData;
    }
    TVectorSize += 1;
    Data[TVectorSize - 1] = elem;
}
template<class T>
void TVector<T>::PopBack() {
    if (TVectorSize > 0){
        --TVectorSize;
        if (TVectorSize < TVectorCapacity / 2){
            TVectorCapacity /= 2;
            T* newData = new T[TVectorCapacity];
            for (unsigned long long i = 0; i < TVectorSize; ++i){
                newData[i] = Data[i];
            }
            delete [] Data;
            Data = newData;
        }
    }
}
template<class T>
void TVector<T>::Insert(const unsigned long long &pos, const T &elem) {
    if (pos > TVectorSize){
        std::cout << "incorrect position to insrt\n";
        return;
    }
    PushBack(T());
    for (unsigned long long i = TVectorSize - 1; i > pos; --i){
        Data[i] = Data[i-1];
    }
    Data[pos] = elem;
}
template<class T>
void TVector<T>::OrdinaryInsert(const T &elem) {
    unsigned long long l = 0;
    unsigned long long r = TVectorSize;
    unsigned long long m;
    while (l < r){
        m = (l + r) / 2;
        if (Data[m] < elem){
            l = m + 1;
        } else {
            r = m;
        }
    }
    Insert(l, elem);
}
template<class T>
void TVector<T>::Erase(const unsigned long long int &pos) {
    if (pos >= TVectorSize){
        std::cout << "incorrect position to insrt\n";
        return;
    }
    for (unsigned long long i = pos; i < TVectorSize - 1; ++i){
        Data[i] = Data[i+1];
    }
    PopBack();
}
template<class T>
void TVector<T>::OrdinaryErase(const T &elem) {
    unsigned long long l = 0;
    unsigned long long r = TVectorSize;
    unsigned long long m;
    while (l < r){
        m = (l + r) / 2;
        if (Data[m] < elem){
            l = m + 1;
        } else {
            r = m;
        }
    }
    Erase(l);
}
template <class T>
T& TVector<T>::operator[] (const long long iterator){
    return Data[iterator];
}
template <class T>
TVector<T>::~TVector(){
    delete [] Data;
}
#endif
