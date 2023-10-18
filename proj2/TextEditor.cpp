//
// Created by Zhou on 16/10/2023.
//
#include "iostream"
#include "fstream"
#include "list"
using namespace std;
/**
 * R：读取文本文件到缓冲区中，缓冲区中以前的任何内
容将丢失，当前行是文件的第一行。\n
W：将缓冲区的内容写入文本文件，当前行或缓冲区
均不改变。\n
I：插入单个新行，用户必须在恰当的提示符的响应中
键入新行并提供其行号。\n
D：删除当前行并移到下一行。\n
F：从当前行开始，查找包含有用户请求的目标串的第
一行。\n
C：将用户请求的字符串修改成用户请求的替换字符串
，仅在当前行中有效。\n
Q：退出编辑器。\n
H：显示解释所有命令的帮助消息，程序也接受?作为
H的替代者。\n
N：下一行，在缓冲区中进一行。\n
P：上一行，在缓冲区中退一行。\n
B：开始，到缓冲区的第一行。\n
E：结束，到缓冲区的最后一行。\n
G：转到缓冲区中用户指定的行号。\n
V：查看缓冲区的全部内容，显示到终端上。\n
Undo：撤销，即回到上一步操作之前的状态。\n
Redo：重做，即撤销后可以恢复撤销前的状态。\n
 */
//const size_t Command_R=0;
//const size_t Command_W=1;
//const size_t Command_I=2;
//const size_t Command_D=3;
//const size_t Command_F=4;
//const size_t Command_C=5;
//const size_t Command_Q=6;
//const size_t Command_H=7;
//const size_t Command_N=8;
//const size_t Command_P=9;
//const size_t Command_B=10;
//const size_t Command_E=11;
//const size_t Command_G=12;
//const size_t Command_V=13;
//const size_t Command_Undo=14;
//const size_t Command_Redo=15;

class textEditor{
private:
    list<string>text;
    string path;
    fstream fs;
    int currentLine;
    void stringToLower(string&s){
        for (char & ch:s) {
            if(ch>='A'&&ch<='Z')
                ch= tolower(ch);
        }
    }
    void commandRead(){
        cout<<"Read From File\n";
        fs.open(path);
        if (path.empty())
            throw runtime_error("Open Empty File");
        if (!fs.is_open())
            throw runtime_error("Cannot Open File");
        text.clear();
        string line;
        while (fs>>line){
            text.push_back(line);
        }
        currentLine=0;
        fs.close();
        cout<<"Read OK !\n";
    }
    void commandWrite(){
        cout<<"Write To File\n";
        fs.open(path);
        if (!fs.is_open())
            throw runtime_error("Cannot Open File");
        for (string line:text) {
            fs<<line;
        }
        fs.close();
        cout<<"Write OK !\n";
    }
    void commandInsert(){
        cout<<"Insert New Line\n";
        cout<<"Insert line after line ?\n  > ";
        int line_n=0;
        cin>>line_n;
        if (!cin||line_n<0||line_n>=text.size())
            throw runtime_error("Input line number error");
        cout<<"New line ?\n  > ";
        string line;
        cin>>line;
        if (!cin)
            throw runtime_error("Input new line error");
        auto iter=text.begin();
        for (int i = 0; i < line_n-1; ++i) {
            ++iter;
        }
        text.insert(iter,line);
        currentLine=line_n+1;
        cout<<"Insert OK !\n";
    }
    void commandDelete(){
        cout<<"Delete Line\n";
        cout<<"Delete Line After Line ?\n  > ";
        int line_n=0;
        cin>>line_n;
        if (!cin||line_n<0||line_n>=text.size())
            throw runtime_error("Input line number error");
        auto iter=text.begin();
        for (int i = 0; i < line_n-1; ++i) {
            ++iter;
        }
        text.erase(iter);
        currentLine=line_n-1;
        cout<<"Delete OK !\n";
    }
    void commandFind(){}
    void commandChange(){}
    void commandQuit(){
        cout<<"GOODBYE !\nSEE YOU NEXT TIME !\n";
        exit(0);
    }
    void commandHelp(){
        cout<<"Help Message\n"
        <<"R：读取文本文件到缓冲区中，缓冲区中以前的任何内"
          "容将丢失，当前行是文件的第一行。\n"
          "W：将缓冲区的内容写入文本文件，当前行或缓冲区"
          "均不改变。\n"
          "I：插入单个新行，用户必须在恰当的提示符的响应中"
          "键入新行并提供其行号。\n"
          "D：删除当前行并移到下一行。\n"
          "F：从当前行开始，查找包含有用户请求的目标串的第"
          "一行。\n"
          "C：将用户请求的字符串修改成用户请求的替换字符串"
          "，仅在当前行中有效。\n"
          "Q：退出编辑器。\n"
          "H：显示解释所有命令的帮助消息，程序也接受?作为"
          "H的替代者。\n"
          "N：下一行，在缓冲区中进一行。\n"
          "P：上一行，在缓冲区中退一行。\n"
          "B：开始，到缓冲区的第一行。\n"
          "E：结束，到缓冲区的最后一行。\n"
          "G：转到缓冲区中用户指定的行号。\n"
          "V：查看缓冲区的全部内容，显示到终端上。\n"
          "Undo：撤销，即回到上一步操作之前的状态。\n"
          "Redo：重做，即撤销后可以恢复撤销前的状态。\n";
        cout<<"Help OK !\n";
    }
    void commandNext(){}
    void commandPrevious(){}
    void commandBegin(){}
    void commandEnd(){}
    void commandGo(){}
    void commandView(){}
    void commandUndo(){}
    void commandRedo(){}
public:
    void init(){
        cout<<"---------------------------------------\n"
                "WELCOME !\n"
                "---------------------------------------\n";
        cout<<"File path\n  > ";
        cin>>path;
        fs=fstream{path};
        currentLine=0;
        cout<<"Init OK !\n";
    }
    void getCommand(){
        cout<<"  > ";
        string command;
        cin>>command;
        stringToLower(command);
        if (command=="r")
            commandRead();
        else if (command=="w")
            commandWrite();
        else if (command=="i")
            commandInsert();
        else if(command=="d")
            commandDelete();
        else if(command=="f")
            commandFind();
        else if(command=="c")
            commandChange();
        else if(command=="q")
            commandQuit();
        else if(command=="h"||command=="?")
            commandHelp();
        else if(command=="n")
            commandNext();
        else if(command=="p")
            commandPrevious();
        else if(command=="b")
            commandBegin();
        else if(command=="e")
            commandEnd();
        else if(command=="g")
            commandGo();
        else if(command=="v")
            commandView();
        else if(command=="undo")
            commandUndo();
        else if(command=="redo")
            commandRedo();
        else
            throw runtime_error("Input command illegal !");
    }
};
int main(){
    textEditor t;
    t.init();
    while (true){
        try {
            t.getCommand();
        } catch (exception&e){
            cerr<<e.what()<<endl;
        }
    }
}