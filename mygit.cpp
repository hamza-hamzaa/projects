#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include "SHA256.cpp"
#include<cstdio>
#include <cerrno>
using namespace std;

void createRepo(const string& name) {
    if (mkdir(name.c_str(), 0777) == -1 && errno != EEXIST) {
        cerr << "Could not create " << name << '\n';
        return;
    }
    string objects = name + "/objects";
    if (mkdir(objects.c_str(), 0777) == -1 && errno != EEXIST) {
        cerr << "Could not create objects directory\n";
        return;
    }
}
bool exists(const string& fileName){
    ifstream index(".mygit/index.txt");
    string line;
    while (getline(index, line)){
        size_t separator = line.find(" : ");
        if (separator == string::npos)
            continue;
        string storedFileName = line.substr(0, separator);
        if (storedFileName == fileName)
            return true;
    }
    return false;
}
vector<string> indexCommitFiles(){
    vector<string> files;
    ifstream index(".mygit/index.txt");
    string line;
    while (getline(index, line)){
        size_t separator = line.find(" : ");
        if (separator == string::npos)
            continue;
        string storedFileName = line.substr(3+separator);
        files.push_back(storedFileName);
    }
    return files;
}
vector<string> ParentCommitFiles(string fileName){
    ifstream in(".mygit/objects/"+fileName);
    vector<string> files;
    if (!in) {return files;}
    string line;
    int lineNumber = 0;
    while (getline(in, line)){
        ++lineNumber;
        if (lineNumber <= 7) {continue;}
        size_t separator = line.find(" : ");
        if (separator == string::npos) {continue;}
        string storedFileName = line.substr(3+separator);
        files.push_back(storedFileName);
    }
    return files;
}
vector<string> getDiffferentFileNmes(vector<string>parentCommits, vector<string>indexCommits){
    vector<string>diff(0);
    bool found=false;
    for(int i=0;i<indexCommits.size();i++){
        found=false;
        for(int j=0;j<parentCommits.size();j++){
            if(indexCommits[i]==parentCommits[j]){found=true;}
        }
        if(!found){
            diff.push_back(indexCommits[i]);
            
        }
    }
    return diff;
}
string getFileName(string hash){
    ifstream index(".mygit/index.txt");
    string line;
    while (getline(index, line)){
        size_t separator = line.find(" : ");
        if (separator == string::npos)
            continue;
        string hshedName = line.substr(3+separator);
        string fileName = line.substr(0,separator);
        if(hash==hshedName){
            return fileName;
        }
        else{
            continue;
        }
    }
    return string();


}

void INDEX_FILE(string fileName){
    if(!exists(fileName)){
        ofstream index(".mygit/index.txt", ios::app);
        index<<fileName<<" : "<<hashFile(fileName)<<endl;
    }
    else{
        return;
    }
}
void commitChanges(string message){
    ifstream INHEAD(".mygit/HEAD");
    ofstream tempcommit(".mygit/objects/tempcommit");
    if(!INHEAD){
        tempcommit<<"message:"<<endl;
        tempcommit<<message<<endl<<endl;
        tempcommit<<"PARENT: "<<endl;
        tempcommit<<"NONE"<<endl<<endl;
        tempcommit<<"FILES:"<<endl;
        ifstream index(".mygit/index.txt", ios::binary);
        char c;
        while (index.get(c)){
            tempcommit.put(c);
        }
    tempcommit.close();
    string fileName=hashFile(".mygit/objects/tempcommit");
    ofstream finalcommit(".mygit/objects/"+fileName);
    ifstream in(".mygit/objects/tempcommit", ios::binary);
    
    while (in.get(c)){
        finalcommit.put(c);
    }
    tempcommit.close();
    remove(".mygit/objects/tempcommit");
    ofstream head(".mygit/HEAD");
    head<<fileName;

    }
    else{
        string parent;
        getline(INHEAD,parent);
        vector<string>indexFiles=indexCommitFiles();
        vector<string> ParentFiles=ParentCommitFiles(parent);
        vector<string> differentCommits=getDiffferentFileNmes(ParentFiles,indexFiles);
        if(differentCommits.size()==0){
            cout<<"nothing to commit"<<endl;
            return;
        }
        tempcommit<<"message:"<<endl;
        tempcommit<<message<<endl<<endl;
        tempcommit<<"PARENT: "<<endl;
        tempcommit<<parent<<endl<<endl;
        tempcommit<<"FILES:"<<endl;
        for(int i=0;i<differentCommits.size();i++){
            tempcommit<<getFileName(differentCommits[i])<<" : "<<differentCommits[i]<<endl;
        }
        tempcommit.close();
        string fileName=hashFile(".mygit/objects/tempcommit");
        ofstream commit(".mygit/objects/"+fileName);
        ifstream in(".mygit/objects/tempcommit", ios::binary);
        char c;
        while (in.get(c)){
            commit.put(c);
        }
        tempcommit.close();
        remove(".mygit/objects/tempcommit");
        ofstream head(".mygit/HEAD");
        head<<fileName;
        }

    }
    

void commit(string message){
    ifstream INHEAD(".mygit/HEAD");
    ofstream tempcommit(".mygit/objects/tempcommit");
    if(!INHEAD){
        tempcommit<<"message:"<<endl;
        tempcommit<<message<<endl<<endl;
        tempcommit<<"PARENT: "<<endl;
        tempcommit<<"NONE"<<endl<<endl;
        tempcommit<<"FILES:"<<endl;
        ifstream index(".mygit/index.txt", ios::binary);
        char c;
        while (index.get(c)){
            tempcommit.put(c);
        }
    tempcommit.close();
    string fileName=hashFile(".mygit/objects/tempcommit");
    ofstream finalcommit(".mygit/objects/"+fileName);
    ifstream in(".mygit/objects/tempcommit", ios::binary);
    
    while (in.get(c)){
        finalcommit.put(c);
    }
    tempcommit.close();
    remove(".mygit/objects/tempcommit");
    ofstream head(".mygit/HEAD");
    head<<fileName;


    }
    else{
        string parent;
        getline(INHEAD,parent);
        tempcommit<<"message:"<<endl;
        tempcommit<<message<<endl<<endl;
        tempcommit<<"PARENT: "<<endl;
        tempcommit<<parent<<endl<<endl;
        tempcommit<<"FILES:"<<endl;
        ifstream index(".mygit/index.txt", ios::binary);
        char c;
        while (index.get(c)){
            tempcommit.put(c);
        }
        tempcommit.close();
        string fileName=hashFile(".mygit/objects/tempcommit");
        ofstream finalcommit(".mygit/objects/"+fileName);
        ifstream in(".mygit/objects/tempcommit", ios::binary);
        
        while (in.get(c)){
            finalcommit.put(c);
        }
        tempcommit.close();
        remove(".mygit/objects/tempcommit");
        ofstream head(".mygit/HEAD");
        head<<fileName;
    }
}

void addFile(string fileName){
    string hash=hashFile(fileName);
    ifstream check(".mygit/objects/" + hash, ios::binary);
    if(!check){
        ifstream in(fileName, ios::binary);
        ofstream out(".mygit/objects/" + hash, ios::binary);
        char c;
        while (in.get(c)){
            out.put(c);
        }
        INDEX_FILE(fileName);       

    } 

}



int main(){
    createRepo(".mygit");
    addFile("barcode.cpp");
   
    
}