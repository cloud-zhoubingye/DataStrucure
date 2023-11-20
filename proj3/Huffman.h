//
// Created by Zhou on 20/11/2023.
//

#ifndef DATASTRUCTURE_HUFFMAN_H
#define DATASTRUCTURE_HUFFMAN_H

#include <map>
#include <iostream>
#include <fstream>
using namespace std;
class HuffTree{
public:
    int _weight;
    int _parent;
    int _leftChild;
    int _rightChild;
    HuffTree(): _weight(0), _parent(0),
                _leftChild(0), _rightChild(0) {}

    static void SelectMinFreq(HuffTree* tree, int index,int& index1,int&index2){
        int compare=9999;
        for (int i = 1; i <= index; ++i) {
            if (tree[i]._parent!=0)
                if (tree[i]._weight<compare){
                    compare=tree[i]._weight;
                    index1=i;
                }
        }
        compare=9999;
        for (int i = 1; i <= index; ++i) {
            if (tree[i]._parent!=0&&i!=index1&&tree[i]._weight>=tree[index1]._weight){
                if (tree[i]._weight<compare){
                    compare=tree[i]._weight;
                    index2=i;
                }
            }
        }
    }
};

HuffTree* CreateHuffmanTree(map<char,int>& frequency){
    HuffTree* tree=new HuffTree[2*frequency.size()];
    int i0 = 1;
    for (auto iter=frequency.begin(); i0 <= frequency.size(); ++i0,++iter) {
        tree[i0]._weight=iter->second;
    }
    for (; i0 <= 2 * frequency.size()-1; ++i0) {
        int index1,index2;
        HuffTree::SelectMinFreq(tree,i0-1,index1,index2);
        tree[index1]._parent=i0;
        tree[index2]._parent=i0;
        tree[i0]._leftChild=index1;
        tree[i0]._rightChild=index2;
        tree[i0]._weight=tree[index1]._weight+tree[index2]._weight;
    }
    return tree;
}

string CreateHuffmanCode(HuffTree* tree, int index) {
    string code;
    int i=index-1;
    int parentIndex=tree[i]._parent;
    while (parentIndex!=0){
        if (tree[parentIndex]._leftChild==i)
            code='0'+code;
        else code='1'+code;
        i=parentIndex;
        parentIndex=tree[i]._parent;
    }
    return code;
}
#endif //DATASTRUCTURE_HUFFMAN_H
