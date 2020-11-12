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

struct Node{
    TVector<Item> Data;
    TVector<Node*> Childs;
    Node* Parent;
    ~Node(){
    }
};

void printBTree (Node* treeNode,int cnt){
    if (treeNode != nullptr){
        for (int i=0; i<treeNode->Data.Size(); ++i){
            printBTree(treeNode->Childs[i], cnt+1);
            for (int j=0; j<4*cnt; ++j){
                std::cout << " ";
            }
            std::cout << treeNode->Data[i].Key << " " << "\n";
        }
        printBTree(treeNode->Childs[treeNode->Childs.Size()-1], cnt+1);
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

void AddToTree (Node* &Root, Item it){
    Node* treeNode = Root;
    while (treeNode -> Childs[0] != nullptr){
        unsigned long long l = 0;
        unsigned long long r = treeNode->Data.Size();
        unsigned long long m;
        while (l < r){
            m = (l + r) / 2;
            if (treeNode -> Data[m] < it){
                l = m + 1;
            } else {
                r = m;
            }
        }
        treeNode = treeNode -> Childs[l];
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
}



int main() {
    Node* bTree = new Node;
    bTree -> Childs.PushBack(nullptr);
    Item tmp;
    tmp.Value=0;
    while (std::cin >> tmp.Key[0]){
        std::cout << "\n";
        AddToTree(bTree, tmp);
        printBTree(bTree, 0);

    }
    return 0;
}
