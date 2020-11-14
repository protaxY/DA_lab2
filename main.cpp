#include <iostream>
#include "TVector.h"
#include "TVector.cpp"
#include <queue>
const unsigned long long t = 2;
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
            if (treeNode -> Parent != nullptr){
                std::cout << treeNode->Parent->Data[0].Key << "-";
            }
            std::cout << treeNode->Data[i].Key << " " << "\n";

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

//Item SearchInTree (Node* &treeNode){
//
//}


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
            return false;
        }
        treeNode = treeNode -> Childs[l];
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
                            treeNode -> Childs[l] -> Parent = treeNode -> Childs[l];
                            treeNode -> Childs[l] -> Childs.PushBack(treeNode -> Childs[l + 1] -> Childs[0]);
                            treeNode -> Childs[l] -> Childs[treeNode -> Childs[l] -> Childs.Size() - 1] -> Parent = treeNode -> Childs[l];
                            treeNode -> Data[l] = treeNode -> Childs[l + 1] -> Data[0];
                            treeNode -> Childs[l + 1] -> Data.Erase(0);
                            treeNode -> Childs[l + 1] -> Childs.PopBack();
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
        tmp.Value = 0;
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
            if (treeNode -> Data[l] == it){
                break;
            }
            treeNode = treeNode -> Childs[l];
        }
        treeNode -> Data[l] = tmp;
    }
    return true;
}

int main() {
    Node* bTree = new Node;
    bTree -> Childs.PushBack(nullptr);
    Item tmp;
    tmp.Value=0;
    char function[KEY_SIZE + 1];
    while (std::cin >> function){
        if (function[0] == '+'){
            std::cin >> tmp.Key >> tmp.Value;
            if (AddToTree(bTree, tmp)){
                std::cout << "OK\n";
            } else {
                std::cout << "Exist\n";
            }
        } else if (function[0] == '-'){
            std::cin >> tmp.Key;
            if (DeleteFromTree(bTree, tmp)){
                std::cout << "OK\n";
            } else {
                std::cout << "NoSuchWord\n";
            }
        } else {

        }
    }

    goAround(bTree);
    delete bTree;
    return 0;
}
