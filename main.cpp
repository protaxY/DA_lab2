#include "BTree.h"

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

int main(){
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    BTree::Node<Item>* bTree = new BTree::Node<Item>;
    bTree -> Childs.PushBack(nullptr);
    char function[KEY_SIZE + 1];
    for (int i = 0; i < KEY_SIZE + 1; ++i){
        function[i] = 0;
    }
    while (std::cin >> function){
        Item tmp;
        if (function[0] == '+'){
            std::cin >> tmp.Key >> tmp.Value;
            for (int i = 0; i < KEY_SIZE; ++i){
                if (tmp.Key[i] >= 'A' && tmp.Key[i] <= 'Z'){
                    tmp.Key[i] = tmp.Key[i] - 'A' + 'a';
                }
            }
            if (BTree::AddToTree(bTree, tmp)){
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
            if (BTree::DeleteFromTree<Item>(bTree, tmp)){
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
                    std::cout << "ERROR:\n";
                    continue;
                }
                BTree::SaveTreeInFile<Item>(bTree, saveFile);
                fclose(saveFile);
                std::cout << "OK\n";
            } else {
                std::cin >> function;
                FILE* loadFile = fopen(function, "rb");
                if (loadFile == nullptr){
                    std::cout << "ERROR:\n";
                    continue;
                }
                char tmp;
                fread(&tmp, sizeof(char), 1, loadFile);
                delete bTree;
                bTree = BTree::LoadTreeFromFile<Item>(nullptr, loadFile);
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
            Item* result = BTree::SearchInTree<Item>(bTree, target);
            if (result != nullptr){
                std::cout << "OK: " << result->Value << "\n";
            } else {
                std::cout << "NoSuchWord\n";
            }
        }
        for (int i = 0; i < KEY_SIZE + 1; ++i){
            function[i] = 0;
        }
    }
    delete bTree;
    return 0;
}
