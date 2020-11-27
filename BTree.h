#define DA_LAB2_BTREE_H
#include "TVector.h"

namespace BTree {
    const unsigned long long t = 8;
    const unsigned int KEY_SIZE = 256;
    template <class T>
    struct Node{
        TVector<T> Data;
        TVector<Node<T>*> Childs;
        Node<T>* Parent = nullptr;
        ~Node(){
            for (int i = 0; i < Childs.Size(); ++i){
                delete Childs[i];
            }
        };
    };
    template <class T>
    T* SearchInTree (Node<T>* treeNode, T it){
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
    }
    template <class T>
    void Split (Node<T>* &treeNode){
        if (treeNode->Parent == nullptr){
            Node<T>* newNode = new Node<T>;
            newNode -> Parent = nullptr;
            newNode -> Data.PushBack(treeNode -> Data[(2 * t - 1) / 2]);
            Node<T>* newNodeChildLeft = new Node<T>;
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
            Node<T>* newNodeChildRight = new Node<T>;
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
            Node<T>* oldTreeNode = treeNode;
            treeNode = treeNode -> Parent;
            for (int i = 0; i < oldTreeNode -> Childs.Size(); ++i){
                oldTreeNode -> Childs[i] = nullptr;
            }
            delete oldTreeNode;
        } else {
            Node<T>* newNodeChildLeft = new Node<T>;
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
            Node<T>* newNodeChildRight = new Node<T>;
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
            T tmp = treeNode -> Data[(2 * t - 1) / 2];
            Node<T>* oldTreeNode = treeNode;
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
    template <class T>
    bool AddToTree (Node<T>* &Root, T it){
        Node<T>* treeNode = Root;
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
    template <class T>
    bool DeleteFromTree (Node<T>* &Root, T it){
        Node<T>* treeNode = Root;
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
                        Node<T>* oldRoot = treeNode;
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
            Node<T> *newCandidate;
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
            T tmp = newCandidate -> Data[newCandidate -> Data.Size() - 1];
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
    template <class T>
    void SaveTreeInFile(Node<T>* treeNode, FILE* saveFile){
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
    template <class T>
    Node<T>* LoadTreeFromFile(Node<T>* parent, FILE* loadFile){
        char bracket;
        fread(&bracket, sizeof(char), 1, loadFile);
        Node<T>* treeNode = new Node<T>;
        fread(&bracket, sizeof(char), 1, loadFile);
        while (bracket != ']'){
            T tmp;
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
};

#endif
