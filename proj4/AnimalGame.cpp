//
// Created by Zhou on 27/11/2023.
//
#include "DecisionTree.h"
void Save(DecisionTree tree){
    cout<<"Enter history directory path :\n> ";
    string path;
    cin>>path;
    tree.Save(path + "\\history.txt");
}
void Guess(BTNode* cur){
    cout<<"Do the animal is "<<cur->getData()<<" ? (y/n)\n> ";
    char ch=0;
    cin>>ch;
    ch= tolower(ch);
    if (ch=='y'){
        cout<<"NICE ! I win the game !\n";
    } else if (ch=='n'){
        cout<<"Sorry ! I will learn it right now !\nCould you tell me what is it ?\n";
        string knowledge;
        cin>>knowledge;
        cur->setData(knowledge);
        cout<<"Thanks ! I've learned it !\n";
    } else throw runtime_error("Input char illegal !");
}
void PlayGame(DecisionTree tree) {
    cout << "Let's Begin !\nImagine an animal in your mind, then let me guess !\n";
    BTNode* cur=tree.getRoot();
    for (int i = 0; i<2; ++i) {
        cout<<Question[i]<<"> ";
        char ch=0;
        cin>>ch;
        ch= tolower(ch);
        if (ch=='y'){
            cur=cur->getRightChild();
        } else if (ch=='n'){
            cur=cur->getLeftChild();
        } else throw runtime_error("Input char illegal !");
    }
    Guess(cur);
}
int main(){
    while (true){
        try {
            DecisionTree tree;
            tree.Init();
            cout<<"Welcome !\n";
            while (true){
                PlayGame(tree);
                cout<<"Try again(y) or exit(n) ?(y/n)\n> ";
                char ch=0;
                cin>>ch;
                ch= tolower(ch);
                if (ch=='y'){
                    continue;
                } else if (ch=='n'){
                    Save(tree);
                    break;
                } else throw runtime_error("Input char illegal !");
            }
            cout<<"Goodbye !\n";
            return 0;
        } catch (exception &exception) {
            cout<<"ERROR!\n"<<exception.what()<<endl;
        }
    }
}