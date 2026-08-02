
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include<fstream>

using namespace std;


void createRepo(string name){
    char* charName=new char(name[0]);
    for(int i=1;i<name.length();i++){
        *(charName+i)=name[i];
    }   
    if(mkdir(charName, 0777)==-1){
        return;
    }

}
void addFile(string fileName){
    ofstream out(".mygit/"+fileName);

}


int main(){
    createRepo(".mygit");
    addFile("hamza.c");
}