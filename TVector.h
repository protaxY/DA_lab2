#ifndef LAB1_TVector_H
#define LAB1_TVector_H
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
    void Erase(const unsigned long long &pos);
    T& operator[] (long long iterator);
    ~TVector();
};
#endif