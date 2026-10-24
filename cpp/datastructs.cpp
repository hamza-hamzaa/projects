// i created this before starting my ADS course to practice my current data structures

#include<iostream>
#include<stdexcept>
using namespace std;

template<typename T>
struct LLNode{
    T val;
    LLNode* next;
    LLNode(T v):val(v),next(nullptr){}
};
template<typename T>
struct TNode{
    T val;
    TNode* parent;
    TNode* right;
    TNode* left;
    TNode(T v):val(v),parent(nullptr),right(nullptr),left(nullptr){}

};



template<typename T>
class HASHTABLE{
    private:
        int type;
        T* hashTableP;
        LLNode<T>** hashTableC;

        int size;
        int numElements;
        float alpha;
        int hashFunc(T val){
            return val%size;
        }
        int hashFunc2(T val){
            return val%(size-1);
        }
        void resize(){
            int oldSize=size;
            size+=1;
            T* oldhashTableP=hashTableP;
            hashTableP=new T[size];
            for(int i=0;i<size;i++){
                hashTableP[i]=-9999;
            }
            numElements=0;
            for(int i=0;i<oldSize;i++){
                if (oldhashTableP[i] != -9999) {
                   insert(oldhashTableP[i]);
                }
                    
            }
            
            delete[] oldhashTableP;
            
        }


        void insertChain(T val){
            int idx=hashFunc(val);
            if(hashTableC[idx]==nullptr){
                hashTableC[idx]=new LLNode<T>(val);
                numElements++;
            }
            else{
                LLNode<T>* newNode=new LLNode<T>(val);
                LLNode<T>* temp=hashTableC[idx];
                LLNode<T>* prev=nullptr;
                if(temp->val>val){
                    newNode->next=temp;
                    hashTableC[idx]=newNode;
                    numElements++;
                }
                else{
                    while(temp!=nullptr&& temp->val<val){
                        prev=temp;
                        temp=temp->next;
                    }
                    if(temp!=nullptr){
                        prev->next=newNode;
                        newNode->next=temp;
                        numElements++;
                    }
                    else{
                        prev->next=newNode;
                        numElements++;
                    }
                
                }

            }
        }
        void insertLinearProbe(T val){
            
            alpha=static_cast<float>(numElements)/size;
            if(alpha>=0.7){
                this->resize();
            }
            int idx=hashFunc(val);
            int initialIdx=hashFunc(val);
            int turn=0;
            bool notFound=true;
            while(notFound&&turn<size){
                if(hashTableP[idx]==-9999){
                    hashTableP[idx]=val;
                    notFound=false;
                    numElements++;            
                    return ;
                }
                else{
               
                    idx=hashFunc(initialIdx+turn++);   
                }
            }
            return ;
        }
        void insertQuadraticProbe(T val){
            alpha=static_cast<float>(numElements)/size;
            if(alpha>=0.7){
                this->resize();
            }
            int idx= hashFunc(val);
            int initialIdx=hashFunc(val);
            int turn=0;
            bool notFound=true;
            while(notFound&&turn<size){
                if( hashTableP[idx]==-9999){
                    hashTableP[idx]=val;
                    notFound=false;
                    numElements++;
                    return ;
                }
                else{
                    turn++;
                    idx=hashFunc(initialIdx+turn*turn);
                }
            }
            return ;

        }
        void insertDoubleProbe(T val){
            alpha=static_cast<float>(numElements)/size;
            if(alpha>=0.7){
                this->resize();
            }
            int idx= hashFunc(val);
            int turn=0;
            bool notFound=true;
            while(notFound&&turn<size){
                if( hashTableP[idx]==-9999){
                    hashTableP[idx]=val;
                    notFound=false;
                    numElements++;
                    return ;
                }
                else{
                    turn++;
                    idx=hashFunc(val+turn*hashFunc2(val));
                }
            }
            return ;
        }
        void chainPrint(){
            for(int i=0;i<size;i++){
                if(hashTableC[i]==nullptr){
                    cout<<" "<<endl;
                }
                else{
                    LLNode<int>* temp=hashTableC[i];
                    while(temp!=nullptr){
                        cout<<temp->val<<" ";
                        temp=temp->next;
                    }
                    cout<<endl;
                }
            }
        }
        void probePrint(){
            for(int i=0;i<size;i++){
                if(hashTableP[i]==-9999){
                    cout<<" "<<endl;
                }
                else{
                    cout<<hashTableP[i]<<endl;
                }
            }
        }
    public:
        HASHTABLE(int probe):type(probe),size(10),numElements(0){
            if(type==1){
                hashTableC= new LLNode<T>*[size];
                for(int i=0;i<size;i++){
                    hashTableC[i]=nullptr;
                }
            }
            else{
                hashTableP=new T[size];
                for(int i=0;i<size;i++){
                    hashTableP[i]=-9999;
                }
            }
        }
        void insert(T val){
            if(type==1){
                insertChain(val);
                return;
            }
            else if(type==2){
                insertLinearProbe(val);
                return;
            }
            else if(type==3){
                insertQuadraticProbe(val);
                return;
            }
            else{
                insertDoubleProbe(val);
                return;
            }
        }
        void print(){
            if(type==1){
                this->chainPrint();
                return;
            }
            else{
                this->probePrint();
                return;
            }
        }
        int getElementsNum(){return numElements;}

        void remove(T val){
            if(type==1){
                removeChain(val);
            }
            else{
                removeProbe(val);
            }
        }

};

template<typename T>
class Queue{
    private:
        int size;
        LLNode<T>* head;
    public:
        Queue():size(0){
            head= new LLNode<T>(0);

        }
        void push(T val){
            
            if(size==0){
                head->val=val;
                size++;
                return;
            }
            LLNode<T>* temp=head;
            while(temp->next!=nullptr){
                temp=temp->next;
            }
            temp->next=new LLNode<T>(val);
            size++;
        }
        T front(){return head->val;}
        T back(){
            LLNode<T>* temp=head;
            while(temp->next!=nullptr){
                temp=temp->next;
            }
            return temp->val;
        }
        bool isEmpty(){return size==0;}
        int getSize(){return size;}
        void pop(){
            if(size==0){return;}
            LLNode<T>* temp=head;
            head=head->next;
            delete temp;
            size--;
        }
};

template<typename T>
class BinaryTree{
    protected:
        TNode<T>* root;
        void insert(T val, TNode<T>*& root,TNode<T>* parent){
            if(root==nullptr){
                root= new TNode<T>(val);
                root->parent=parent;
            }
            else{
                if(root->val>val){
                    insert(val, root->left,root);
                }
                else{
                    insert(val,root->right,root);
                }
            }
        }
        void inOrderPrint(TNode<T>* root){
            if(root==nullptr){return;}
            inOrderPrint(root->left);
            cout<<root->val<<" ";
            inOrderPrint(root->right);
        }

        void preOrderPrint(TNode<T>* root){
            if(root==nullptr){return;}
            cout<<root->val<<" ";
            preOrderPrint(root->left);
            preOrderPrint(root->right);
        }

        void postOrderPrint(TNode<T>* root){
            if(root==nullptr){return;}
            postOrderPrint(root->left);
            postOrderPrint(root->right);
            cout<<root->val<<" ";;
        }
        bool search(T val,TNode<T>* root){
            if(root==nullptr){return false;}
            if(root->val==val){return true;}
            else{
                if(root->val>val){
                    return search(val,root->left);
                }
                else{
                    return search(val,root->right);
                }
            }
        }
        TNode<T>* searchN(T val,TNode<T>* root){
            if(root==nullptr){return nullptr;}
            if(root->val==val){return root;}
            else{
                if(root->val>val){
                    return searchN(val,root->left);
                }
                else{
                    return searchN(val,root->right);
                }
            }
        }

        T max(TNode<T>* root){
            if(root->right==nullptr){return root->val;}
            else{
                return max(root->right);
            }
            
        }
        T min(TNode<T>* root){
            if(root->left==nullptr){return root->val;}
            else{
                return min(root->left);
            }
        }
        void clear(TNode<T>* root){
            if(root==nullptr){return;}
            clear(root->left);
            clear(root->right);
            delete root;
        }
        void remove(T val){
            if(!search(val)){return;}
            TNode<T>* delNode=searchN(val,root);
            if(delNode->left==nullptr&& delNode->right==nullptr){
                TNode<T>* p=delNode->parent;
                if(p->val>val){
                    p->left=nullptr;
                    delete delNode;
                    return;
                }
                else{
                    p->right=nullptr;
                    delete delNode;
                    return;
                }
            }
            if(delNode->left==nullptr){
                T newVal=min(delNode->right);
                delNode->val=newVal;
                delNode=delNode->right;
                while(delNode->left!=nullptr){
                    delNode=delNode->left;
                }
                delNode->parent->left=nullptr;
                delete delNode;
                return;
            }
            else{
                T newVal=max(delNode->left);
                delNode->val=newVal;
                delNode=delNode->left;
                while(delNode->right!=nullptr){
                    delNode=delNode->right;
                }
                delNode->parent->right=nullptr;
                delete delNode;
                return;
            }
        }
    public:
        BinaryTree(T val){
            root = new TNode<T>(val);
        }
        void insert(T val){
            insert(val,root,nullptr);
        }
        void print(){
            postOrderPrint(root);
        }
        bool search(T val){
            return search(val,root);
        }
        T max(){
            return max(root);
        }
        T min(){
            return min(root);
        }
        void clear(){
            clear(root);
            root=nullptr;
        }

};

template<typename T>
class PriorityQueue{
    private:
        LLNode<T>* head;
        int size;
        
    public:
       
        PriorityQueue():head(nullptr),size(0){}
        void push(T val){
            if(size==0){
                head=new LLNode<T>(val);
                size++;
                return;
            }
            LLNode<T>* temp=head;
            LLNode<T>* prev=nullptr;
            LLNode<T>* newNode=new LLNode<T>(val);
            while(temp!=nullptr&&temp->val>val){
                prev=temp;
                temp=temp->next;
            }
            if (prev==nullptr){
                newNode->next=temp;
                head=newNode;
                size++;
            }
            else{
                prev->next=newNode;
                newNode->next=temp;

                size++;
            }
        }
        void print(){
            LLNode<T>* temp=head;
            while(temp!=nullptr){
                cout<<temp->val<<" ";
                temp=temp->next;
            }

        }
        T top(){
            if(empty()){throw runtime_error("empty");}
            return head->val;}
        bool empty(){return size==0;}
        int getSize(){return size;}
        void pop(){
            if(head == nullptr){return;}
            LLNode<T>* temp=head;
            head=head->next;
            delete temp;
            size--;
        }   
    
};




template<typename T>
class Stack{
    private:
        LLNode<T>*  head;

    public:
        Stack():head(nullptr){}
        void push(T val){
            if(head==nullptr){
                head= new LLNode<T>(val);
            }
            else{
                LLNode<T>* temp=new LLNode<T>(val);
                temp->next=head;
                head=temp;
            }
        }
        void pop(){
            if(head==nullptr){return;}
            else{
                LLNode<T>* temp=head;
                head=head->next;
                delete temp;
            }
            
        }
        T top(){
            if(head!=nullptr){return head->val;}
            throw runtime_error("empty stack");
        }
};
