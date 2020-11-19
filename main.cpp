#include <iostream>
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
    void OrdinaryErase (const T &elem);
    void Erase(const unsigned long long &pos);
    T& operator[] (long long iterator);
    ~TVector();
};
#endif

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
const unsigned long long t = 8;
const unsigned int KEY_SIZE = 256;

struct Item{
    char Key[KEY_SIZE];
    unsigned long long Value;
    Item(){
        for (int i = 0; i < KEY_SIZE; ++i){
            Key[i] = 0;
        }
        Value = 0;
    }
    Item& operator= (const Item it){
        for (unsigned long long i = 0; i < KEY_SIZE; ++i){
            Key[i] = it.Key[i];
        }
        Value = it.Value;
        return *this;
    }
};

bool operator< (const Item &lhs, const Item &rhs){
    for (int i = 0; i < KEY_SIZE; ++i){
        if (lhs.Key[i] < rhs.Key[i]){
            return true;
        } else if (lhs.Key[i] > rhs.Key[i]) {
            return false;
        }
    }
    return false;
}

bool operator== (const Item &lhs, const Item &rhs){
    for (int i = 0; i < KEY_SIZE; ++i){
        if (lhs.Key[i] != rhs.Key[i]){
            return false;
        }
    }
    return true;
}

struct Node{
    TVector<Item> Data;
    TVector<Node*> Childs;
    Node* Parent = nullptr;
    ~Node(){
        for (int i = 0; i < Childs.Size(); ++i){
            delete Childs[i];
        }
    };
};

void printBTree (Node* treeNode,int cnt){
    if (treeNode != nullptr){
        for (int i=0; i<treeNode->Data.Size(); ++i){
            printBTree(treeNode->Childs[i], cnt+1);
            for (int j=0; j<4*cnt; ++j){
                std::cout << " ";
            }
            std::cout << treeNode -> Data[i].Key << " " << treeNode -> Data[i].Value  << "\n";

        }
        printBTree(treeNode->Childs[treeNode->Childs.Size()-1], cnt+1);
    }
}

void goAround (Node* &treeNode){
    if (treeNode -> Childs[0] != nullptr){
        for (int i = 0; i < treeNode -> Childs.Size(); ++i){
            treeNode = treeNode -> Childs[i];
            goAround(treeNode);
        }
    }
    if (treeNode -> Parent != nullptr){
        treeNode = treeNode -> Parent;
    }
}

void Split (Node* &treeNode){
    if (treeNode->Parent == nullptr){
        Node* newNode = new Node;
        newNode -> Parent = nullptr;
        newNode -> Data.PushBack(treeNode -> Data[(2 * t - 1) / 2]);
        Node* newNodeChildLeft = new Node;
        for (int i = 0; i < (2 * t - 1) / 2; ++i){
            newNodeChildLeft -> Data.PushBack(treeNode -> Data[i]);
            newNodeChildLeft -> Childs.PushBack((treeNode -> Childs[i]));
        }
        newNodeChildLeft -> Parent = newNode;
        newNodeChildLeft -> Childs.PushBack(treeNode -> Childs[(2 * t - 1) / 2]);
        if (newNodeChildLeft -> Childs[0] != nullptr){
            for (int i = 0; i < newNodeChildLeft -> Childs.Size(); ++i){
                newNodeChildLeft -> Childs[i] -> Parent = newNodeChildLeft;
            }
        }
        Node* newNodeChildRight = new Node;
        for (int i = (2 * t - 1) / 2 + 1; i < (2 * t - 1); ++i){
            newNodeChildRight -> Data.PushBack(treeNode -> Data[i]);
            newNodeChildRight -> Childs.PushBack(treeNode -> Childs[i]);
        }
        newNodeChildRight -> Parent = newNode;
        newNodeChildRight -> Childs.PushBack(treeNode -> Childs[2 * t - 1]);
        newNode -> Childs.PushBack(newNodeChildLeft);
        newNode -> Childs.PushBack(newNodeChildRight);
        if (newNodeChildRight -> Childs[0] != nullptr){
            for (int i = 0; i < newNodeChildRight -> Childs.Size(); ++i){
                newNodeChildRight -> Childs[i] -> Parent = newNodeChildRight;
            }
        }
        treeNode -> Parent = newNode;
        Node* oldTreeNode = treeNode;
        treeNode = treeNode -> Parent;
        for (int i = 0; i < oldTreeNode -> Childs.Size(); ++i){
            oldTreeNode -> Childs[i] = nullptr;
        }
        delete oldTreeNode;
    } else {
        Node* newNodeChildLeft = new Node;
        for (int i = 0; i < (2 * t - 1) / 2; ++i){
            newNodeChildLeft -> Data.PushBack(treeNode -> Data[i]);
            newNodeChildLeft -> Childs.PushBack(treeNode -> Childs[i]);
        }
        newNodeChildLeft -> Parent = treeNode -> Parent;
        newNodeChildLeft -> Childs.PushBack(treeNode -> Childs[(2 * t - 1) / 2]);
        if (newNodeChildLeft -> Childs[0] != nullptr){
            for (int i = 0; i < newNodeChildLeft -> Childs.Size(); ++i){
                newNodeChildLeft -> Childs[i] -> Parent = newNodeChildLeft;
            }
        }
        Node* newNodeChildRight = new Node;
        for (int i = (2 * t - 1) / 2 + 1; i < (2 * t - 1); ++i){
            newNodeChildRight -> Data.PushBack(treeNode -> Data[i]);
            newNodeChildRight -> Childs.PushBack(treeNode -> Childs[i]);
        }
        newNodeChildRight -> Parent = treeNode -> Parent;
        newNodeChildRight -> Childs.PushBack(treeNode -> Childs[2 * t - 1]);
        if (newNodeChildRight -> Childs[0] != nullptr){
            for (int i = 0; i < newNodeChildRight -> Childs.Size(); ++i){
                newNodeChildRight -> Childs[i] -> Parent = newNodeChildRight;
            }
        }
        Item tmp = treeNode -> Data[(2 * t - 1) / 2];
        Node* oldTreeNode = treeNode;
        treeNode = treeNode -> Parent;
        for (int i = 0; i < oldTreeNode -> Childs.Size(); ++i){
            oldTreeNode -> Childs[i] = nullptr;
        }
        delete oldTreeNode;
        if (treeNode -> Data.Size() == 2 * t - 2){
            unsigned long long l = 0;
            unsigned long long r = treeNode -> Data.Size();
            unsigned long long m;
            while (l < r){
                m = (l + r) / 2;
                if (treeNode -> Data[m] < tmp){
                    l = m + 1;
                } else {
                    r = m;
                }
            }
            treeNode -> Data.Insert(l, tmp);
            treeNode -> Childs[l] = newNodeChildLeft;
            treeNode -> Childs.Insert(l + 1, newNodeChildRight);
            Split(treeNode);

        } else {
            unsigned long long l = 0;
            unsigned long long r = treeNode -> Data.Size();
            unsigned long long m;
            while (l < r){
                m = (l + r) / 2;
                if (treeNode -> Data[m] < tmp){
                    l = m + 1;
                } else {
                    r = m;
                }
            }
            treeNode -> Data.Insert(l, tmp);
            treeNode -> Childs[l] = newNodeChildLeft;
            treeNode -> Childs.Insert(l + 1, newNodeChildRight);
        }
    }
}

bool AddToTree (Node* &Root, Item it){
    Node* treeNode = Root;
    while (treeNode -> Childs[0] != nullptr){
        unsigned long long l = 0;
        unsigned long long r = treeNode->Data.Size();
        unsigned long long m;
        while (l < r){
            m = (l + r) / 2;
            if (treeNode -> Data[m] < it){
                l = m + 1;
            }
            else {
                r = m;
            }
        }
        if (l < treeNode -> Data.Size() && treeNode -> Data[l] == it){
            return false;
        }
        treeNode = treeNode -> Childs[l];
    }
    unsigned long long l = 0;
    unsigned long long r = treeNode->Data.Size();
    unsigned long long m;
    while (l < r){
        m = (l + r) / 2;
        if (treeNode -> Data[m] < it){
            l = m + 1;
        }
        else {
            r = m;
        }
    }
    if (l < treeNode -> Data.Size() && treeNode -> Data[l] == it){
        return false;
    }
    if (treeNode -> Data.Size() < 2 * t - 2){
        treeNode -> Data.OrdinaryInsert(it);
        treeNode -> Childs.PushBack(nullptr);
    } else {
        treeNode -> Data.OrdinaryInsert(it);
        treeNode -> Childs.PushBack(nullptr);
        Split(treeNode);
    }
    while (treeNode -> Parent != nullptr){
        treeNode = treeNode -> Parent;
    }
    Root = treeNode;
    return true;
}

Item* SearchInTree (Node* treeNode, Item it){
    while (treeNode != nullptr){
        long long l = -1;
        long long r = treeNode->Data.Size();
        long long m;
        while (l + 1 < r){
            m = (l + r) / 2;
            if (treeNode -> Data[m] < it){
                l = m;
            }
            else {
                r = m;
            }
        }
        if (r < treeNode -> Data.Size() && treeNode -> Data[r] == it){
            return &(treeNode->Data[r]);
        }
        treeNode = treeNode -> Childs[r];
    }
    if (treeNode == nullptr){
        return nullptr;
    }
    return nullptr;
}

bool DeleteFromTree (Node* &Root, Item it){
    Node* treeNode = Root;
    while (treeNode != nullptr){
        unsigned long long l = 0;
        unsigned long long r = treeNode->Data.Size();
        unsigned long long m;
        while (l < r){
            m = (l + r) / 2;
            if (treeNode -> Data[m] < it){
                l = m + 1;
            }
            else {
                r = m;
            }
        }
        if (l < treeNode -> Data.Size() && treeNode -> Data[l] == it){
            break;
        }
        treeNode = treeNode -> Childs[l];
    }
    if (treeNode == nullptr){
        return false;
    }
    if (treeNode -> Childs[0] == nullptr){
        if (treeNode -> Data.Size() > t - 1 || treeNode -> Parent == nullptr){
            treeNode -> Data.OrdinaryErase(it);
            treeNode -> Childs.PopBack();
        } else {
            treeNode -> Data.OrdinaryErase(it);
            treeNode -> Childs.PopBack();
            treeNode = treeNode -> Parent;
            unsigned long long l = 0;
            unsigned long long r = treeNode->Data.Size();
            unsigned long long m;
            while (l < r){
                m = (l + r) / 2;
                if (treeNode -> Data[m] < it){
                    l = m + 1;
                }
                else {
                    r = m;
                }
            }
            if (l != 0 && treeNode -> Childs[l - 1] -> Data.Size() > t - 1){
                treeNode -> Childs[l] -> Data.Insert(0, treeNode -> Data[l - 1]);
                treeNode -> Childs[l] -> Childs.PushBack(nullptr);
                treeNode -> Data[l - 1] = treeNode -> Childs[l - 1] -> Data[treeNode -> Childs[l - 1] -> Data.Size() - 1];
                treeNode -> Childs[l - 1] -> Data.Erase(treeNode -> Childs[l - 1] -> Data.Size() - 1);
                treeNode -> Childs[l - 1] -> Childs.PopBack();
            } else if (l + 1 < treeNode -> Childs.Size() && treeNode -> Childs[l + 1] -> Data.Size() > t - 1){
                treeNode -> Childs[l] -> Data.PushBack(treeNode -> Data[l]);
                treeNode -> Childs[l] -> Childs.PushBack(nullptr);
                treeNode -> Data[l] = treeNode -> Childs[l + 1] -> Data[0];
                treeNode -> Childs[l + 1] -> Data.Erase(0);
                treeNode -> Childs[l + 1] -> Childs.PopBack();
            } else {
                unsigned long long l = 0;
                unsigned long long r = treeNode->Data.Size();
                unsigned long long m;
                while (l < r){
                    m = (l + r) / 2;
                    if (treeNode -> Data[m] < it){
                        l = m + 1;
                    }
                    else {
                        r = m;
                    }
                }
                if (l != 0){
                    treeNode -> Childs[l - 1] -> Data.PushBack(treeNode -> Data[l - 1]);
                    for (int i = 0; i < treeNode -> Childs[l] -> Data.Size(); ++i){
                        treeNode -> Childs[l - 1] -> Data.PushBack(treeNode -> Childs[l] -> Data[i]);
                        treeNode -> Childs[l - 1] -> Childs.PushBack(nullptr);
                    }
                    treeNode -> Childs[l - 1] -> Childs.PushBack(nullptr);
                    treeNode -> Data.Erase(l - 1);
                    delete treeNode -> Childs[l];
                    treeNode -> Childs.Erase(l);
                } else if (l + 1 < treeNode -> Childs.Size()){
                    treeNode -> Childs[l] -> Data.PushBack(treeNode -> Data[l]);
                    for (int i = 0; i < treeNode -> Childs[l + 1] -> Data.Size(); ++i){
                        treeNode -> Childs[l] -> Data.PushBack(treeNode -> Childs[l + 1] -> Data[i]);
                        treeNode -> Childs[l] -> Childs.PushBack(nullptr);
                    }
                    treeNode -> Childs[l] -> Childs.PushBack(nullptr);
                    treeNode -> Data.Erase(l);
                    delete treeNode -> Childs[l + 1];
                    treeNode -> Childs.Erase(l + 1);
                }
                while (treeNode -> Data.Size() == t - 2 && treeNode -> Parent != nullptr){
                    treeNode = treeNode -> Parent;
                    unsigned long long l = 0;
                    unsigned long long r = treeNode -> Data.Size();
                    unsigned long long m;
                    while (l < r){
                        m = (l + r) / 2;
                        if (treeNode -> Data[m] < it){
                            l = m + 1;
                        }
                        else {
                            r = m;
                        }
                    }
                    if (l != 0 && treeNode -> Childs[l - 1] -> Data.Size() > t - 1){
                        treeNode -> Childs[l] -> Data.Insert(0, treeNode -> Data[l - 1]);
                        treeNode -> Childs[l] -> Childs.Insert(0, treeNode -> Childs[l - 1] -> Childs[treeNode -> Childs[l - 1] -> Childs.Size() - 1]);
                        treeNode -> Childs[l] -> Childs[0] -> Parent = treeNode -> Childs[l];
                        treeNode -> Data[l - 1] = treeNode -> Childs[l - 1] -> Data[treeNode -> Childs[l - 1] -> Data.Size() - 1];
                        treeNode -> Childs[l - 1] -> Data.Erase(treeNode -> Childs[l - 1] -> Data.Size() - 1);
                        treeNode -> Childs[l - 1] -> Childs.PopBack();
                    } else if (l + 1 < treeNode -> Childs.Size() && treeNode -> Childs[l + 1] -> Data.Size() > t - 1){
                        treeNode -> Childs[l] -> Data.PushBack(treeNode -> Data[l]);
                        treeNode -> Childs[l] -> Childs.PushBack(treeNode -> Childs[l + 1] -> Childs[0]);
                        treeNode -> Childs[l] -> Childs[treeNode -> Childs[l] -> Childs.Size() - 1] -> Parent = treeNode -> Childs[l];
                        treeNode -> Data[l] = treeNode -> Childs[l + 1] -> Data[0];
                        treeNode -> Childs[l + 1] -> Data.Erase(0);
                        treeNode -> Childs[l + 1] -> Childs.Erase(0);
                    } else {
                        if (l != 0){
                            treeNode -> Childs[l - 1] -> Data.PushBack(treeNode -> Data[l - 1]);
                            for (int i = 0; i < treeNode -> Childs[l] -> Data.Size(); ++i){
                                treeNode -> Childs[l - 1] -> Data.PushBack(treeNode -> Childs[l] -> Data[i]);
                                treeNode -> Childs[l - 1] -> Childs.PushBack(treeNode -> Childs[l] -> Childs[i]);
                                treeNode -> Childs[l - 1] -> Childs[treeNode -> Childs[l - 1] -> Childs.Size() - 1] -> Parent = treeNode -> Childs[l - 1];
                                treeNode -> Childs[l] -> Childs[i] = nullptr;
                            }
                            treeNode -> Childs[l - 1] -> Childs.PushBack(treeNode -> Childs[l] -> Childs[treeNode -> Childs[l] -> Childs.Size() - 1]);
                            treeNode -> Childs[l - 1] -> Childs[treeNode -> Childs[l - 1] -> Childs.Size() - 1] -> Parent = treeNode -> Childs[l - 1];
                            treeNode -> Childs[l] -> Childs[treeNode -> Childs[l] -> Childs.Size() - 1] = nullptr;
                            treeNode -> Data.Erase(l - 1);
                            delete treeNode -> Childs[l];
                            treeNode -> Childs.Erase(l);
                        } else if (l + 1 < treeNode -> Childs.Size()){
                            treeNode -> Childs[l] -> Data.PushBack(treeNode -> Data[l]);
                            for (int i = 0; i < treeNode -> Childs[l + 1] -> Data.Size(); ++i){
                                treeNode -> Childs[l] -> Data.PushBack(treeNode -> Childs[l + 1] -> Data[i]);
                                treeNode -> Childs[l] -> Childs.PushBack(treeNode -> Childs[l + 1] -> Childs[i]);
                                treeNode -> Childs[l] -> Childs[treeNode -> Childs[l] -> Childs.Size() - 1] -> Parent = treeNode -> Childs[l];
                                treeNode -> Childs[l + 1] -> Childs[i] = nullptr;
                            }
                            treeNode -> Childs[l] -> Childs.PushBack(treeNode -> Childs[l + 1] -> Childs[treeNode -> Childs[l + 1] -> Childs.Size() - 1]);
                            treeNode -> Childs[l] -> Childs[treeNode -> Childs[l] -> Childs.Size() - 1] -> Parent = treeNode -> Childs[l];
                            treeNode -> Childs[l + 1] -> Childs[treeNode -> Childs[l + 1] -> Childs.Size() - 1] = nullptr;
                            treeNode -> Data.Erase(l);
                            delete treeNode -> Childs[l + 1];
                            treeNode -> Childs.Erase(l + 1);
                        }
                    }
                }
                if (treeNode -> Parent == nullptr && treeNode -> Data.Size() == 0){
                    Node* oldRoot = treeNode;
                    treeNode = treeNode -> Childs[0];
                    treeNode -> Parent = nullptr;
                    for (int i = 0; i < oldRoot -> Childs.Size(); ++i){
                        Root -> Childs[i] = nullptr;
                    }
                    delete Root;
                    Root = treeNode;
                }
            }
        }
    } else {
        unsigned long long l = 0;
        unsigned long long r = treeNode->Data.Size();
        unsigned long long m;
        while (l < r) {
            m = (l + r) / 2;
            if (treeNode->Data[m] < it) {
                l = m + 1;
            } else {
                r = m;
            }
        }
        unsigned long long swapPosition = l;
        Node *newCandidate;
        newCandidate = treeNode->Childs[l];
        while (newCandidate->Childs[0] != nullptr) {
            unsigned long long l = 0;
            unsigned long long r = newCandidate->Data.Size();
            unsigned long long m;
            while (l < r) {
                m = (l + r) / 2;
                if (newCandidate->Data[m] < it) {
                    l = m + 1;
                } else {
                    r = m;
                }
            }
            newCandidate = newCandidate->Childs[l];
        }
        Item tmp = newCandidate -> Data[newCandidate -> Data.Size() - 1];
        DeleteFromTree(Root, tmp);
        treeNode = Root;
        while (treeNode != nullptr){
            unsigned long long l = 0;
            unsigned long long r = treeNode->Data.Size();
            unsigned long long m;
            while (l < r){
                m = (l + r) / 2;
                if (treeNode -> Data[m] < it){
                    l = m + 1;
                }
                else {
                    r = m;
                }
            }
            if (l < treeNode -> Data.Size() && treeNode -> Data[l] == it){
                treeNode -> Data[l] = tmp;
                break;
            }
            treeNode = treeNode -> Childs[l];
        }
    }
    return true;
}

void SaveTreeInFile(Node* treeNode, FILE* saveFile){
    if (treeNode == nullptr){
        return;
    }
    fwrite("{", sizeof(char), 1, saveFile);
    fwrite("[", sizeof(char), 1, saveFile);
    for (int i = 0; i < treeNode -> Data.Size(); ++i){
        fwrite("(", sizeof(char), 1, saveFile);
        fwrite(treeNode -> Data[i].Key, sizeof(char), 256, saveFile);
        fwrite(&(treeNode -> Data[i].Value), sizeof(unsigned long long), 1, saveFile);
        fwrite(")", sizeof(char), 1, saveFile);
    }
    fwrite("]", sizeof(char), 1, saveFile);
    for (int i = 0; i < treeNode -> Childs.Size(); ++i){
        SaveTreeInFile(treeNode -> Childs[i], saveFile);
    }
    fwrite("}", sizeof(char), 1, saveFile);
}

//void PrintFile(FILE* file){
//    char bracket;
//    fread(&bracket, sizeof(char), 1, file);
//    std::cout << bracket;
//    fread(&bracket, sizeof(char), 1, file);
//    while (bracket != '}'){
//        std::cout << bracket;
//        fread(&bracket, sizeof(char), 1, file);
//        while (bracket != ']'){
//            std::cout << bracket;
//            Item tmp;
//            fread(&tmp.Key, sizeof(char), KEY_SIZE, file);
//            fread(&tmp.Value, sizeof(unsigned long long), 1, file);
//            std::cout << tmp.Value << "," << tmp.Key;
//            fread(&bracket, sizeof(char), 1, file);
//            std::cout << bracket;
//            fread(&bracket, sizeof(char), 1, file);
//        }
//        fread(&bracket, sizeof(char), 1, file);
//    }
//
//}

Node* LoadTreeFromFile(Node* parent, FILE* loadFile){
    char bracket;
    fread(&bracket, sizeof(char), 1, loadFile);
    Node* treeNode = new Node;
    fread(&bracket, sizeof(char), 1, loadFile);
    while (bracket != ']'){
        Item tmp;
        fread(&tmp.Key, sizeof(char), KEY_SIZE, loadFile);
        fread(&tmp.Value, sizeof(unsigned long long), 1, loadFile);
        treeNode -> Data.PushBack(tmp);
        treeNode -> Childs.PushBack(nullptr);
        fread(&bracket, sizeof(char), 1, loadFile);
        fread(&bracket, sizeof(char), 1, loadFile);
    }
    treeNode -> Childs.PushBack(nullptr);
    treeNode -> Parent = parent;
    fread(&bracket, sizeof(char), 1, loadFile);
    if (bracket == '}'){
        return treeNode;
    } else {
        for (int i = 0; i < treeNode->Childs.Size(); ++i){
            treeNode -> Childs[i] = LoadTreeFromFile(treeNode, loadFile);
            fread(&bracket, sizeof(char), 1, loadFile);
        }
        return treeNode;
    }

}

int main(){
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    Node* bTree = new Node;
    bTree -> Childs.PushBack(nullptr);
    Item tmp;
    tmp.Value=0;
    char function[KEY_SIZE + 1];
    for (int i = 0; i < KEY_SIZE + 1; ++i){
        function[i] = 0;
    }
    while (std::cin >> function){
        if (function[0] == '+'){
            std::cin >> tmp.Key >> tmp.Value;
            for (int i = 0; i < KEY_SIZE; ++i){
                if (tmp.Key[i] >= 'A' && tmp.Key[i] <= 'Z'){
                    tmp.Key[i] = tmp.Key[i] - 'A' + 'a';
                }
            }
            if (AddToTree(bTree, tmp)){
                std::cout << "OK\n";
            } else {
                std::cout << "Exist\n";
            }
        } else if (function[0] == '-'){
            std::cin >> tmp.Key;
            for (int i = 0; i < KEY_SIZE; ++i){
                if (tmp.Key[i] >= 'A' && tmp.Key[i] <= 'Z'){
                    tmp.Key[i] = tmp.Key[i] - 'A' + 'a';
                }
            }
            if (DeleteFromTree(bTree, tmp)){
                std::cout << "OK\n";
            } else {
                std::cout << "NoSuchWord\n";
            }
        } else if (function[0] == '!'){
            std::cin >> function;
            if (function[0] == 'S'){
                std::cin >> function;
                FILE* saveFile = fopen(function, "wb");
                if (saveFile == nullptr){
                    std::cout << "Error\n";
                }
                SaveTreeInFile(bTree, saveFile);
                fclose(saveFile);
                std::cout << "OK\n";
            } else {
                std::cin >> function;
                FILE* loadFile = fopen(function, "r");
                char tmp;
                fread(&tmp, sizeof(char), 1, loadFile);
                bTree = LoadTreeFromFile(nullptr, loadFile);
                fclose(loadFile);
                std::cout << "OK\n";
            }
        }
        else {
            Item target;
            for (int i = 0; i < KEY_SIZE; ++i){
                if (function[i] >= 'A' && function[i] <= 'Z'){
                    function[i] = function[i] - 'A' + 'a';
                }
                target.Key[i] = function[i];
            }
            Item* result = SearchInTree(bTree, target);
            if (result != nullptr){
                std::cout << "OK: " << result->Value << "\n";
            } else {
                std::cout << "NoSuchWord\n";
            }
        }
        //printBTree(bTree, 0);
        //goAround(bTree);
    }

    delete bTree;
    return 0;
}
