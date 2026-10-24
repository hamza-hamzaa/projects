#include<iostream>
#include<bitset>
#include<unordered_map>
#include<queue>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

//create a tree node that stores everything needed for huffman compression such as the character and its freq and
// all relevent pointers
struct FNode{
    char character;
    int freq;
    FNode* left;
    FNode* right;
    FNode* parent;
    FNode(char c,int f):freq(f),character(c),left(nullptr),right(nullptr),parent(nullptr){}
};
//functor struct that helps priority queue rank different characters 
struct CompareNodes{
    bool operator()(const FNode* a, const FNode* b) const{
        return a->freq > b->freq;
    }
};

//function that helps to determine the frequency of each character in the text
unordered_map<char,int> frequencyGenerator(const string& buffer){
    unordered_map<char,int>frequencies;
    for(int i=0;i<buffer.length();i++){
        frequencies[buffer[i]]++;
    }
    return frequencies;
}
// converts the map into a priority queue in order to be able to rank all chars based on frequency
priority_queue<FNode*,vector<FNode*>,CompareNodes> createPriorityQueue(const unordered_map<char,int>& freq){
    ////here priiority queue takes addvantage of STL's abilty to use functors to create the priority ordering
    priority_queue<FNode*,vector<FNode*>,CompareNodes> q;
    for (const auto& i : freq){
        q.push(new FNode(i.first, i.second));
    }
    return q;
}

/* creates tree by first taking the two smallest element in the queue and making them leaves then making 
their parent have a alue equal to the sum of both his children then he pushes new value into queue and 
keeps repeating until only one value is left in the queue which is the root of the tree
*/
FNode* createTree(priority_queue<FNode*, vector<FNode*>, CompareNodes> queue) {
    if (queue.empty()){return nullptr;}
    while (queue.size() > 1) {
        FNode* pop1 = queue.top();
        queue.pop();
        FNode* pop2= queue.top();
        queue.pop();
        int num = pop1->freq + pop2->freq;
        FNode* head = new FNode('\0', num);
        head->left = pop1;
        head->right = pop2;
        pop1->parent = head;
        pop2->parent = head;
        queue.push(head);
    }
    return queue.top();
}


//
void generateCodes(FNode* root,const string& currentCode,unordered_map<char, string>& codes){
    if (root == nullptr)
        return;
    bool isLeaf =root->left == nullptr &&root->right == nullptr;
    if (isLeaf){
        codes[root->character] =currentCode.empty() ? "0" : currentCode;
        return;
    }
    generateCodes(root->left, currentCode + "0", codes);
    generateCodes(root->right, currentCode + "1", codes);
}
//after getting the huffman code it converts each character to said code
string encode(const string& buffer,unordered_map<char,string>codes){
    string encodedText;
    for(int i=0;i<buffer.length();i++){
        encodedText+=string(codes[buffer[i]]);
    }
    return encodedText;
}



int main(){
    string text;
    unordered_map<char,int> freq=frequencyGenerator(text); 
    unordered_map<char,string> codes;
    priority_queue<FNode*,vector<FNode*>,CompareNodes>queue=createPriorityQueue(freq);
    FNode* root=createTree(queue);
    generateCodes(root,"",codes);
    string EncodedText=encode(text,codes);

    cout<<EncodedText<<endl;
}