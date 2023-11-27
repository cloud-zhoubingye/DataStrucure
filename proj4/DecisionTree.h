//
// Created by Zhou on 27/11/2023.
//

#ifndef DATASTRUCTURE_DECISIONTREE_H
#define DATASTRUCTURE_DECISIONTREE_H

#include "vector"
#include "iostream"
#include "fstream"
using namespace std;

vector<string> Question={"This animal is a mammal, isn't it?(y/n)\n",
                         "Does this animal fly?(y/n)\n"};

class BTNode{
private:
    string _data;
    BTNode* _leftChild;
    BTNode* _rightChild;
    bool _isLeaf;
public:
    BTNode(): _data(""), _leftChild(nullptr), _rightChild(nullptr), _isLeaf(false) {}

    const string &getData() const {
        return _data;
    }
    void setData(const string &data) {
        BTNode::_data = data;
    }
    BTNode *getLeftChild() const {
        return _leftChild;
    }
    void setLeftChild(BTNode *leftChild) {
        BTNode::_leftChild = leftChild;
    }
    BTNode *getRightChild() const {
        return _rightChild;
    }
    void setRightChild(BTNode *rightChild) {
        BTNode::_rightChild = rightChild;
    }
    bool isIsLeaf() const {
        return _isLeaf;
    }
    void setIsLeaf(bool isLeaf) {
        _isLeaf = isLeaf;
    }
};

class DecisionTree{
private:
    BTNode* _root;
private:
    void ConstructTree(){
        _root->setData(Question[0]);
        BTNode* node1_1=new BTNode();
        node1_1->setData(Question[1]);
        BTNode* node1_2=new BTNode();
        node1_2->setData(Question[1]);
        _root->setLeftChild(node1_1);
        _root->setRightChild(node1_2);
        BTNode* node2_1=new BTNode();
        BTNode* node2_2=new BTNode();
        BTNode* node2_3=new BTNode();
        BTNode* node2_4=new BTNode();
        node1_1->setLeftChild(node2_1);
        node1_1->setRightChild(node2_2);
        node1_2->setLeftChild(node2_3);
        node1_2->setRightChild(node2_4);
    }
    void ReadHistory(string path){
        ConstructTree();
        fstream fs{path+"\\history.txt"};
        if (!fs) throw runtime_error(path+"\\history.txt"+"File Open Failed !");
        string code;
        while (fs>>code){
            BTNode* cur=_root;
            string animal;
            fs>>animal;
            if (code[0]=='0')
                cur=cur->getLeftChild();
            else cur=cur->getRightChild();
            if (code[1]=='0')
                cur=cur->getLeftChild();
            else cur=cur->getRightChild();
            cur->setData(animal);
            cur->setIsLeaf(true);
        }
        fs.close();
    }
public:
    BTNode *getRoot() {
        return _root;
    }
    DecisionTree() {
        _root=new BTNode();
    }
    void Init(){
        cout<<"Enter history directory path :\n> ";
        string path;
        cin>>path;
        ReadHistory(path);
    }
    void Save(string path){
        fstream fs{path};
        if (!fs) throw runtime_error(path+"\\history.txt"+"File Open Failed !");
        fs<<"00 "<<_root->getLeftChild()->getLeftChild()<<endl;
        fs<<"01 "<<_root->getLeftChild()->getRightChild()<<endl;
        fs<<"10 "<<_root->getRightChild()->getLeftChild()<<endl;
        fs<<"11 "<<_root->getRightChild()->getRightChild()<<endl;
    }
};


#endif //DATASTRUCTURE_DECISIONTREE_H
