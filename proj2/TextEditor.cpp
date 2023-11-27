//
// Created by Zhou on 16/10/2023.
//
#include "iostream"
#include "fstream"
#include "vector"
#include "list"
#include "stack"
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


const string helpMessage= "R：读取文本文件到缓冲区中，缓冲区中以前的任何内"
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
                          "Undo：撤销，即回到上一步操作之前的状态(不包含移动当前行号)。\n"
                          "Redo：重做，即撤销后可以恢复撤销前的状态(不包含移动当前行号)。\n";

class textEditor{
private:
    list<string>text;
    string path;
    fstream fs;
    int currentLine;
    bool isSaved;
    bool automaticSaving;
    stack<list<string>> undoStack;
    stack<list<string>> redoStack;

    void Save(){
        fs.open(path);
        if (!fs.is_open())
            throw runtime_error("Cannot Open File");
        for (string line:text) {
            fs<<line;
        }
        fs.close();
        isSaved= true;
    }
    vector<int> computePace(string targetString) {
        int targetLength = targetString.length();
        vector<int> pace(targetLength, 0);
        int len = 0;
        int i = 1;
        while (i < targetLength) {
            if (targetString[i] == targetString[len]) {
                len++;
                pace[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = pace[len - 1];
                } else {
                    pace[i] = 0;
                    i++;
                }
            }
        }
        return pace;
    }
    void KMP(string sourceString, string targetString) {
        int text_length = sourceString.length();
        int pattern_length = targetString.length();

        vector<int> lps = computePace(targetString);

        int i = 0; // index for sourceString
        int j = 0; // index for targetString
        while (i < text_length) {
            if (targetString[j] == sourceString[i]) {
                j++;
                i++;
            }
            if (j == pattern_length) {
                cout << "Pattern found at index " << i - j << endl;
                j = lps[j - 1];
            } else if (i < text_length && targetString[j] != sourceString[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i = i + 1;
                }
            }
        }
    }
    void stringToLower(string&s){
        for (char & ch:s) {
            if(ch>='A'&&ch<='Z')
                ch= tolower(ch);
        }
    }
    void commandRead(){
        fs=fstream(path);
        cout<<"Read From File\n";
        fs.open(path);
        if (path.empty())
            throw runtime_error("Open Empty File");
        if (!fs.is_open())
            throw runtime_error("Cannot Open File");
        string line;
        while (getline(fs,line)){
            text.push_back(line);
        }
        currentLine=0;
//        fs.close();
        cout<<"Read OK !\n";
    }
    void commandWrite(){
        cout<<"Write To File\n";
        Save();
        cout<<"Write OK !\n";
    }
    void commandInsert(){
        cout<<"Insert New Line\n";
        cout<<"Insert line after line ?\n  > ";
        int line_n=0;
        cin>>line_n;
        if (!cin||line_n<0||line_n>text.size())
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
        isSaved= false;
        undoStack.push(text);
        if(automaticSaving) Save();
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
        for (int i = 0; i < line_n; ++i) {
            ++iter;
        }
        text.erase(iter);
        currentLine=line_n-1;
        isSaved= false;
        undoStack.push(text);
        if(automaticSaving) Save();
        cout<<"Delete OK !\n";
    }
    void commandFind(){
        cout<<"Find from a line\n";
        cout<<"Enter Line Number :\n  > ";
        cin >> currentLine;
        if (!cin||currentLine <= 0 || currentLine > text.size())
            throw runtime_error("Input line number error");
        auto line_iter=text.begin();
        for (int i = 0; i < currentLine - 1; ++i) {
            ++line_iter;
        }
        string line=*line_iter;
        string pattern_str;
        cout<<"Enter pattern string :\n  > ";
        cin >> pattern_str;
        cout<<"CurrentLine:  "<<line<<endl;
        KMP(line,pattern_str);
        cout<<"Find OK !\n";
    }
    void commandChange(){
        cout<<"Change certain sub string\n";
        cout<<"Enter Line Number :\n  > ";
        cin >> currentLine;
        if (currentLine < 0 || currentLine > text.size())
            throw runtime_error("Input line number error");
        auto line_iter=text.begin();
        for (int i = 0; i < currentLine - 1; ++i) {
            ++line_iter;
        }
        string line=*line_iter;
        cout<<"Before Change :  "<<line<<endl;
        string pattern_str,after_string;
        cout<<"Enter pattern string :\n  > ";
        cin >> pattern_str;
        cout<<"Enter after_string :\n  > ";
        cin >> after_string;
        line.replace(line.find(pattern_str),pattern_str.size(),after_string);
        cout<<"After Change :  "<<line<<endl;
        *line_iter=line;
        isSaved= false;
        undoStack.push(text);
        if(automaticSaving) Save();
        cout<<"Change OK !\n";
    }
    void commandQuit(){
        cout<<"Quit Programme\n";
        if(automaticSaving) Save();
        if (!isSaved&&!automaticSaving){
            cout<<"Not Saved ! Are you sure to quit ?  y/n\n  > ";
            char ch=0;
            cin>>ch;
            if (!cin)
                throw runtime_error("Input Error! Either 'y' nor 'n' !");
            if (tolower(ch)=='y'){
                //保存临时文件
                cout<<"We've write it to Temp File !\n";
                fs.open(path+"_temp",ios_base::out);
                if (!fs.is_open())
                    throw runtime_error("Cannot Open File"+path+"_temp");
                for (string line:text) {
                    fs<<line;
                }
                fs.close();
                cout<<"Write OK ! Please Open \""+path+"_temp\" to Check !\n";
            }
            else if (tolower(ch)=='n'){
                cout<<"Stop Quit !\n";
                return;
            } else{
                throw runtime_error("Input Error! Either 'y' nor 'n' !");
            }
        }
        cout<<"GOODBYE !\nSEE YOU NEXT TIME !\n";
        exit(0);
    }
    void commandHelp(){
        cout << "Help Message\n" << helpMessage;
        cout<<"Help OK !\n";
    }
    void commandNext(){
        cout<<"Move to Next Line\n";
        if (currentLine+1>text.size())
            throw runtime_error("Already Move to THE END !\n");
        ++currentLine;
        cout<<"Line "<<currentLine<<" :"<<endl;
        auto iter=text.begin();
        for (int i = 0; i < currentLine-1; ++i) {
            ++iter;
        }
        cout<<*iter<<endl;
        cout<<"Move OK !\n";
    }
    void commandPrevious(){
        cout<<"Move to Previous Line\n";
        if (currentLine-1<0)
            throw runtime_error("Already Move to THE BEGIN !\n");
        --currentLine;
        cout<<"Line "<<currentLine<<" :"<<endl;
        auto iter=text.begin();
        for (int i = 0; i < currentLine-1; ++i) {
            ++iter;
        }
        cout<<*iter<<endl;
        cout<<"Move OK !\n";
    }
    void commandBegin(){
        cout<<"Move to First Line\n";
        currentLine=0;
        cout<<"Line "<<currentLine<<" :"<<endl;
        auto iter=text.begin();
        cout<<*iter<<endl;
        cout<<"Move OK !\n";
    }
    void commandEnd(){
        cout<<"Move to Last Line\n";
        currentLine=text.size()-1;
        cout<<"Line "<<currentLine<<" :"<<endl;
        auto iter=text.end();
        --iter;
        cout<<*iter<<endl;
        cout<<"Move OK !\n";
    }
    void commandGo(){
        cout<<"Move to Certain Line\n";
        cout<<"You'd like move to WHICH line ?\n";
        int line_num=0;
        cin>>line_num;
        if (!cin||line_num<0||line_num>=text.size())
            throw runtime_error("Input should be a VALID number !");
        currentLine=line_num;
        cout<<"Line "<<currentLine<<" :"<<endl;
        auto iter=text.begin();
        for (int i = 0; i < currentLine-1; ++i) {
            ++iter;
        }
        cout<<*iter<<endl;
        cout<<"Move OK !\n";
    }
    void commandView(){
        cout<<"View Buffer Content\n";
        auto iter=text.begin();
        for (int i = 0; i < currentLine-1; ++i,++iter) {
            cout<<"Line "<<i+1<<" :"<<endl;
            cout<<*iter<<endl;
        }
        cout<<"View OK !\n";
    }
    void commandUndo(){
        cout<<"Back to Latest Record\n";
        cout<<"Are you sure to UNDO ? (y/n)\n  > ";
        char ch=0;
        cin>>ch;
        if (!cin)
            throw runtime_error("Input Error! Either 'y' nor 'n' !");
        if (tolower(ch)=='y'){
            if (undoStack.empty())
                throw runtime_error("No Operation Done ! Cannot Undo !\n");
            //使用两个栈，undo存储当前做过的操作情况，redo存储当前撤销过的操作情况
            redoStack.push(text);
            text=undoStack.top();
            undoStack.pop();
            isSaved= false;
            if(automaticSaving) Save();
        }
        else if (tolower(ch)=='n'){
            cout<<"Stop Undo !\n";
            return;
        } else{
            throw runtime_error("Input Error! Either 'y' nor 'n' !");
        }
        cout<<"Undo OK !\n";
    }
    void commandRedo(){
        cout<<"Revocation Undo\n";
        cout<<"Are you sure to REDO ? (y/n)\n  > ";
        char ch=0;
        cin>>ch;
        if (!cin)
            throw runtime_error("Input Error! Either 'y' nor 'n' !");
        if (tolower(ch)=='y'){
            if (redoStack.empty())
                throw runtime_error("No Undo Operation Done ! Cannot Redo !\n");
            //使用两个栈，undo存储当前做过的操作情况，redo存储当前撤销过的操作情况
            text=redoStack.top();
            redoStack.pop();
            isSaved= false;
            if(automaticSaving) Save();
        }
        else if (tolower(ch)=='n'){
            cout<<"Stop Redo !\n";
            return;
        } else{
            throw runtime_error("Input Error! Either 'y' nor 'n' !");
        }
        cout<<"Redo OK !\n";
    }
public:
    void init(){
        cout<<"---------------------------------------\n"
              "WELCOME !\n"
              "---------------------------------------\n";
        cout<<"File path\n  > ";
        cin>>path;
        if (!cin)
            throw runtime_error("File path input error");
        cout<<"File Path : "<<path<<endl;
        cout<<"Automatic Saving On ? (y/n)\n  > ";
        char ch=0;
        cin>>ch;
        if (!cin) throw runtime_error("Input Error! Either 'y' nor 'n' !");
        if (tolower(ch)=='y') automaticSaving= true;
        else if(tolower(ch)=='n') automaticSaving= false;
        else throw runtime_error("Input Error! Either 'y' nor 'n' !");
        fs=fstream{path};
        currentLine=0;
        isSaved= true;
        cout<<"Init OK !\n";
        cout<<"User Guide are as follows :\n"<<helpMessage;
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
            cout<<e.what()<<endl;
        }
    }
}