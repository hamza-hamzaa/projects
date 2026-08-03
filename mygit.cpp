#include <iostream>
#include<vector>
#include<string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include "SHA256.cpp"
#include<cstdio>
#include <cerrno>
#include<unordered_map>
using namespace std;



void createRepo(const string& name) {
    //creates main repo and uses cerrno lib to ensure repo doesnt already exist 
    if (mkdir(name.c_str(), 0777) == -1 && errno != EEXIST) {
        cerr << "Could not create " << name << '\n';
        return;
    }
    //same concept but with the objects subfolder
    string objects = name + "/objects";
    if (mkdir(objects.c_str(), 0777) == -1 && errno != EEXIST) {
        cerr << "Could not create objects directory\n";
        return;
    }
}

int exists(const string& fileName, const string& hash){
    //function checks if file exists by checking if the specific hash already exists or not
    ifstream index(".mygit/index.txt");
    string line;
    while (getline(index, line)){
        // it does this by checking whats after the : sep which is the syntax of index
        size_t separator = line.find(" : ");
        if (separator == string::npos){ continue;}
        string storedFileName = line.substr(0,separator);
        string storedHashName = line.substr(3+separator);
        if (storedHashName == hash&& storedFileName==fileName){
            return 1;
        }
        else if(storedFileName==fileName){
            return 2;
        }
        else if(storedHashName == hash ){
            return 3;
        }  
            
    }
    return 0;
}

vector<string> indexCommitFiles(){
    //function that is a bit similar to exists but it returns a vector of file hashes that exist in index file
    vector<string> files;
    ifstream index(".mygit/index.txt");
    string line;
    while (getline(index, line)){
        size_t separator = line.find(" : ");
        if (separator == string::npos)
            continue;
        string storedFileName = line.substr(3+separator);
        //same parsing technique but here it appends to an array rather than returning true or false
        files.push_back(storedFileName);
    }
    return files;
}

vector<string> ParentCommitFiles(const string& fileName){
    //exact same thing like indexCommitFiles but for Parentcommits
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

vector<string> getDiffferentFileNmes(const vector<string>&parentCommits, const vector<string>&indexCommits){
    //gets the difference between to vectors
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
    //takes in a hash and returns the filename
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

string getFileName(string hash, string searchingFile){
    //takes in a hash and returns the filename
    
    ifstream index(".mygit/objects/"+searchingFile);
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

string getParent(string currentCommit){
    //works by going to the line where parent code is and copies the parentCommits hash then returns it 
    ifstream in(".mygit/objects/"+currentCommit);
    if (!in) {return string();}
    string line;
    int lineNumber = 0;
    while (getline(in, line)){
        ++lineNumber;
        if (lineNumber <= 4) {continue;}
        else{return line;}
    }
   return string();
}

void read(string fileName){
    //function that helps in printing log it helps in printing out commit conntents 
    //function works reecursively by relying on the getparent parent function 
    //prints everything using ifstream then clls itself on its parent and stop condition is when parent is NONE
    if(fileName=="NONE"){return;}
    else{
        
        ifstream in(".mygit/objects/"+fileName, ios::binary);
        ofstream out("test.txt", ios::app);
        cout<<endl<<endl<<"commit: "<<fileName<<endl<<endl;
        char c;
        while (in.get(c)){
            cout<<(c);
        }
        cout<<"---------------------------------------------------------------------------------------------------------------"<<endl;
        read(getParent(fileName));
    }
}

void log(){
    //calls read function and exists in order to abstract some of the processes 
    ifstream HEAD(".mygit/HEAD");
    string finalCommit;
    getline(HEAD,finalCommit);
    read(finalCommit);
}
//===============
void INDEX_FILE(string fileName){

    if(exists(fileName, hashFile(fileName))==0|| exists(fileName, hashFile(fileName))==3){
        ofstream index(".mygit/index.txt", ios::app);
        index<<fileName<<" : "<<hashFile(fileName)<<endl;
    }
    else if(exists(fileName, hashFile(fileName))==2){
        ifstream read(".mygit/index.txt");
        ofstream temp(".mygit/temp.txt");
        string line;
        while (getline(read, line))  {
            size_t separator = line.find(" : ");
            if (separator == string::npos){ continue;}
            string storedFileName = line.substr(0,separator);
            if (storedFileName==fileName)
                temp << fileName<<" : "<<hashFile(fileName)<<endl;
            else
                temp << line << '\n';
        }
        read.close();
        temp.close();
        remove(".mygit/index.txt");
        rename(".mygit/temp.txt", ".mygit/index.txt");
    }
    else{
        return;
    }
}

void commitChanges(string message){
    //function prints commits however this one only prints the difference between the current commit and the previous one
    // has two cases that depend on the existanxce of a parent 
    //1- if parent doesnt exist it prints everything from the index file directly and the norml content 
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
    //2- otherwise if parent exists it goes to parent to check which files exist then print the files that exist
    //in index but not in parent 
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
    // same thing as commit changes but here it prints all of index in both cases 
    //only dif between the two conditions is what is printed in the head section
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
        tempcommit<<endl<<endl;
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
        tempcommit<<endl<<endl;
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

void checkout(string Commit){
    // first i check if this is  valid commit if not i exit
    ifstream commit(".mygit/objects/"+Commit);
    if(!commit){return;}
    //then i get all the files and their coresponding hashes from the commitfile
    vector<string>commitFilesHash=ParentCommitFiles(Commit);
    vector<string>commitFiles(0);
    for(int i=0;i<commitFilesHash.size();i++){
        if (getFileName(commitFilesHash[i],Commit).empty()){
            continue;
        }
        commitFiles.push_back(getFileName(commitFilesHash[i],Commit));
    }
    //finally i go over every file and i modify then by copying the hashfile onto the acttual file
    remove(".mygit/index.txt");
    for(int i=0;i<commitFilesHash.size();i++){
        ifstream readingFile(".mygit/objects/"+commitFilesHash[i], ios::binary);
        ofstream editableFile(commitFiles[i]);
        char c;
        while (readingFile.get(c)){
            editableFile.put(c);
        }
        readingFile.close();
        editableFile.close();
        INDEX_FILE(commitFiles[i]);

    }
    ofstream head(".mygit/HEAD");
    head << Commit;
}

unordered_map<string,string> Hash_FileNamesFromHEAD(){
    ifstream HEAD(".mygit/HEAD");
    string finalCommit;
    getline(HEAD,finalCommit);
    HEAD.close();
    ifstream commit(".mygit/objects/"+finalCommit);
    string line;
    unordered_map<string,string> names;
    while (getline(commit, line)){
        size_t separator = line.find(" : ");
        if (separator == string::npos){ continue;}
        string storedFileName = line.substr(0,separator);
        string storedHashName = line.substr(3+separator);
        names[storedFileName]=storedHashName;
    }
    return names;
}

unordered_map<string,string> Hash_FileNamesFromINDEX(){
    ifstream index(".mygit/index.txt");
    unordered_map<string,string> fileNames;
    if (!index) {return  fileNames;}
    string line;
    int lineNumber = 0;
    while (getline(index, line)){
        ++lineNumber;
        if (lineNumber <= 7) {continue;}
        size_t separator = line.find(" : ");
        if (separator == string::npos) {continue;}
        string storedFileName = line.substr(0,separator);
        string storedHashName = line.substr(3+separator);
        fileNames[storedFileName]=storedHashName;
    }
    return fileNames;
}

void status (){
    vector<string>fileNames=indexCommitFiles();
    for(int i=0;i<fileNames.size();i++){
        fileNames[i]=getFileName(fileNames[i]);
    }
    unordered_map<string,string> headFile=Hash_FileNamesFromHEAD();    
    unordered_map<string,string> indexFile=Hash_FileNamesFromINDEX();
    
    for(int i=0;i<fileNames.size();i++){
        string hash=hashFile(fileNames[i]);
        if(hash==headFile[fileNames[i]]){
            cout<<fileNames[i]<<" : "<<"UNMODIFIED"<<endl;
        }
        else if(hash==indexFile[fileNames[i]]){
            cout<<fileNames[i]<<" : "<<"STAGED"<<endl;
        }
        else{
            cout<<fileNames[i]<<" : "<<"UNSTAGED"<<endl;
        }   
    }
}

void addFile(const string& fileName){
    //checks if files actually exist 
    ifstream sourceCheck(fileName, ios::binary);
    if (!sourceCheck){return;}
    sourceCheck.close();
    //create the files hash so that it can add it to the objects subfolder
    string hash = hashFile(fileName);
    ifstream check(".mygit/objects/" + hash, ios::binary);
    //finally it copies from the original file to the hash
    if (!check){
        ifstream in(fileName, ios::binary);
        ofstream out(".mygit/objects/" + hash, ios::binary);
        char c;
        while (in.get(c)){
            out.put(c);
        }
    }
    //indexes file so it can track which hash belongss to which file 
    INDEX_FILE(fileName);
}

int main(){
    //createRepo(".mygit");
    //addFile("SHA256.cpp");
    ////commit("f");
    ////createRepo(".mygit");
    //addFile("huffman.cpp");
    ////commit("s");
    ////createRepo(".mygit");
    //addFile("barcode.cpp");
    ////commit("t");
    //
    addFile("test.txt");
    //commit("testing ");

    //unordered_map<string,string>t=Hash_FileNamesFromHEAD();
    //for(auto i:t){
    //    cout<<i.first<<" "<<i.second<<endl;
    //}

    status();
    






}