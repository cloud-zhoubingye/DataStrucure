//
// Created by Zhou on 13/11/2023.
//
#include "Huffman.h"
/**
 * Compress
 */
map<char,int> CountFrequency(string path){
    fstream fs{path};
    if (!fs) throw runtime_error(path+"\nFile Open Failed !");
    char ch=0;
    map<char,int> frequency;
    while (fs>>ch){
        frequency[ch]++;
    }
    return frequency;
}
map<char,string> ConstructHuffmanCode(map<char,int>& frequency){
    map<char,string>dict;
    HuffTree* tree= CreateHuffmanTree(frequency);
    int i=1;
    for (auto iter:frequency) {
        dict[iter.first]= CreateHuffmanCode(tree, i);
    }
    return dict;
}
void SaveHuffmanCode(map<char,string>dict,string path){
    path.replace(path.find(".txt"),4,"");
    path+="_HuffmanDict.txt";
    ofstream ofs{path};
    if (!ofs) throw runtime_error(path+"_HuffmanDict.txt\n"+"File Open Failed !");
    for (auto iter : dict) {
        ofs<<iter.first<<"  "<<iter.second<<"  ";
    }
}
string GetContent(string path){
    fstream fs{path};
    if (!fs) throw runtime_error(path+"\nFile Open Failed !");
    string content,line;
    while(getline(fs,line)){
        content=content+line+"\n";
    }
    return content;
}
string ReplaceToCompress(string fileContent,map<char,string>dict){
    string afterReplace;
    for (char ch : fileContent) {
        afterReplace.append(dict[ch]+" ");
    }
    return afterReplace;
}
void SaveCompressedFile(string content,string path){
    path.replace(path.find(".txt"),4,"");
    ofstream fs{path+"_compress.txt"};
    if (!fs) throw runtime_error(path+"_compress.txt"+"\nFile Open Failed !");
    fs<<content;
}

/**
* Decompress
*/
map<char,string> ReadHuffmanCode(string path){
    path.replace(path.find("_compress.txt"),13,"");
    path+="_HuffmanDict.txt";
    ifstream ifs{path};
    if (!ifs) throw runtime_error(path+"\nFile Open Failed !");
    char ch=0;
    string str;
    map<char,string>dict;
    while(!ifs.eof()){
        ifs>>ch>>str;
        dict[ch]=str;
    }
    return dict;
}
map<string,char>Convert(map<char,string>dict){
    map<string,char>convertDict;
    for (auto iter : dict){
        convertDict[iter.second] = iter.first;
    }
    return convertDict;
}
string ReplaceToDecompress(string filePath,map<string,char>convertDict){
    string afterReplace;
    fstream fs{filePath};
    if (!fs) throw runtime_error(filePath+"\nFile Open Failed !");
    string str;
    while (fs>>str){
        afterReplace+=(convertDict[str]);
    }
    return afterReplace;
}
void WriteToFile(string path,string content){
    ofstream fs{path};
    if (!fs) throw runtime_error(path+"\nFile Open Failed !");
    fs<<content;
}

/**
* Main
*/
void Compress(){
    cout<<"Enter file path :\n> ";
    string path;
    cin>>path;
    map<char,int> frequency= CountFrequency(path);
    map<char,string> dict= ConstructHuffmanCode(frequency);
    SaveHuffmanCode(dict,path);
    string content=ReplaceToCompress(GetContent(path), dict);
    SaveCompressedFile(content,path);
    cout<<"Compress OK !\nSee it at "+path.replace(path.find(".txt"),4,"")+"_compress.txt !\n";
}
void Decompress(){
    cout<<"Enter file path :\n> ";
    string path;
    cin>>path;
    map<char,string>dict=ReadHuffmanCode(path);
    map<string,char>convertDict=Convert(dict);
    string content=ReplaceToDecompress(path,convertDict);
    WriteToFile(path.replace(path.find("_compress.txt"),9,""),content);
    cout<<"Decompress OK !\nSee it at "+path+" !\n";
}
int main(){
    while (true){
        try {
            cout<<"\nWelcome to zby's Compress toolbox !\n"
                  "Enter COMPRESS to compress a txt file !\n"
                  "Enter DECOMPRESS to decompress a _compress.txt file !\n"
                  "Enter QUIT to quit programme !\n> ";
            string command;
            cin>>command;
            for (char & ch : command) {
                ch= tolower(ch);
            }
            if (command=="compress")
                Compress();
            else if(command=="decompress")
                Decompress();
            else if(command=="quit"){
                cout<<"See you next time !\n";
                return 0;
            }
            else throw runtime_error("Enter command illegal !");
        } catch (exception&e){
            cout<<"OOPS!\n"<<e.what()<<endl;
        }
    }
}
