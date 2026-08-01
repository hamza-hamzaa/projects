#include<iostream>
#include<bitset>
#include<unordered_map>
#include<queue>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

struct FNode{
    char character;
    int freq;
    FNode* left;
    FNode* right;
    FNode* parent;
    FNode(char c,int f):freq(f),character(c),left(nullptr),right(nullptr),parent(nullptr){}
};
struct CompareNodes{
    bool operator()(const FNode* a, const FNode* b) const{
        return a->freq > b->freq;
    }
};
unordered_map<char,int> frequencyGenerator(string buffer){
    unordered_map<char,int>frequencies;
    for(int i=0;i<buffer.length();i++){
        frequencies[buffer[i]]++;
    }
    return frequencies;
}
priority_queue<FNode*,vector<FNode*>,CompareNodes> createQueue(const unordered_map<char,int>& freq){
    priority_queue<FNode*,vector<FNode*>,CompareNodes> q;
    for (const auto& i : freq){
        q.push(new FNode(i.first, i.second));
    }
    return q;
}
FNode* createTree(priority_queue<FNode*,vector<FNode*>,CompareNodes> queue){
    FNode* pop1;
    FNode* pop2;
    FNode* head;
    //i assume that there are more than 2 elements in the queue
    //i'll fix it later
    pop1=queue.top();
    queue.pop();
    pop2=queue.top();
    queue.pop();
    int num=pop1->freq+pop2->freq;
    head=new FNode('\0',num);
    head->left=pop1;
    head->right=pop2;
    pop1->parent=head;
    pop2->parent=head;
    while(!queue.empty()){
        pop1=queue.top();
        queue.pop();
        pop2=head;
        num=pop1->freq+pop2->freq;
        head=new FNode('\0',num);   
        head->left=pop1;
        head->right=pop2;
        pop1->parent=head;
        pop2->parent=head; 
    }
    return head;
}
void generateCodes(FNode* node,const string& currentCode,unordered_map<char, string>& codes){
    if (node == nullptr)
        return;
    bool isLeaf =node->left == nullptr &&node->right == nullptr;
    if (isLeaf){
        codes[node->character] =currentCode.empty() ? "0" : currentCode;
        return;
    }
    generateCodes(node->left, currentCode + "0", codes);
    generateCodes(node->right, currentCode + "1", codes);
}
string encode(const string& buffer,unordered_map<char,string>codes){
    string encodedText;
    for(int i=0;i<buffer.length();i++){
        encodedText+=string(codes[buffer[i]]);
    }
    return encodedText;
}



int main(){
    string text=" ";
    unordered_map<char,int> freq=frequencyGenerator(text); 
    unordered_map<char,string> codes;
    priority_queue<FNode*,vector<FNode*>,CompareNodes>queue=createQueue(freq);
    FNode* root=createTree(queue);
    generateCodes(root,"",codes);
    string EncodedText=encode(text,codes);


    ofstream out("compressed.huff", ios::binary);


int uniqueChars = freq.size();
out.write(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));
for (const auto& i : freq){
    out.write(&i.first, sizeof(char));
    out.write(reinterpret_cast<const char*>(&i.second), sizeof(int));
}
int bitCount = EncodedText.length();
out.write(reinterpret_cast<char*>(&bitCount), sizeof(bitCount));
unsigned char currentByte = 0;
int bitPos = 7;
for (char bit : EncodedText){
    if (bit == '1')
        currentByte |= (1 << bitPos);
    bitPos--;
    if (bitPos < 0){
        out.write(reinterpret_cast<char*>(&currentByte), 1);
        currentByte = 0;
        bitPos = 7;
    }
}

// Write the final partially filled byte
if (bitPos != 7)
{
    out.write(reinterpret_cast<char*>(&currentByte), 1);
}
out.close();
cout<<EncodedText;
}
