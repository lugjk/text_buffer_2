#include "RopeTextBuffer.h"

// ----------------- DoublyLinkedList -----------------
Rope::Node::Node() {
        left=nullptr;
        right=nullptr;
        data="";
        weight=0;
        height=1;
         balance=EH;
}
Rope::Rope(){root=nullptr;}
Rope::~Rope() {
        destroy(root);
}

Rope::Node* Rope::clone(Node* node) const {
    if (!node) return nullptr;
    Node* temp = new Node();
    temp->data = node->data;
    temp->weight = node->weight;
    temp->height = node->height;
    temp->balance = node->balance;
    temp->left = clone(node->left);
    temp->right = clone(node->right);
    return temp;
}
Rope::Rope(const Rope& other) {
    root = clone(other.root);
}

Rope& Rope::operator=(const Rope& other) {
    if (this != &other) {
        destroy(root);
        root = clone(other.root);
    }
    return *this;
}
bool Rope::operator==(const Rope& other)const {
    return equalNodes(this->root,other.root);
}
bool Rope::equalNodes(Node*a, Node* b) const{
    if(a==nullptr && b==nullptr)return 1;
    if(a==nullptr || b==nullptr)return 0;
    if(a->data!=b->data)return 0;
     if (a->weight != b->weight)   return false;
    if (a->height != b->height)   return false;
    if (a->balance != b->balance) return false;  
    
    return equalNodes(a->left, b->left) && equalNodes(a->right, b->right);
}
Rope::Node::Node(const string& s){
    
            height=0;
        if(s.size()<=Rope::CHUNK_SIZE){
            data=s;
            weight=s.size();
            left=nullptr;
            right=nullptr;
            height=1;
            balance=EH;
        }else{
        left =nullptr;
        right = nullptr;
        weight = 0;
        height = 1;
        balance = EH;
        data = "";
        }
}

bool Rope::Node:: isLeaf() const{
    if(left==nullptr&&right==nullptr){
        return 1;
    }
    else return 0;
}

// TODO: implement other methods of DoublyLinkedList

int Rope::height(Node *node) const
{   if(node==nullptr)return 0;
    return node->height;
}

int Rope::getTotalLength(Node *node) const
{   if(node==nullptr)return 0;
    if(node->isLeaf()){return node->weight;}
    else return(getTotalLength(node->right)+getTotalLength(node->left));
    return 0;
}

void Rope::update(Node *node)
{   if(node==nullptr)return ;
    if(node->isLeaf()){
        node->weight=node->data.size();
        node->height=1;
        node->balance=Node::EH;
        return ;
    }
    int hleft=height(node->left);
    int hright=height(node->right);
    node->weight=getTotalLength(node->left);
    if(hleft>hright){node->height=hleft+1;}
    else node->height=1+hright;
    
    if((hleft-hright)>0)node->balance=Node::LH;
    else{ if(hleft-hright==0)node->balance=Node::EH;
        else node->balance=Node::RH ;}
    
}

Rope::Node*Rope::rotateLeft(Node *x)
{
    if(x==nullptr||x->right==nullptr)return x;
    Node* tempx=x->right;
    Node* tempy=tempx->left;
    tempx->left=x;
    x->right=tempy;
    update(x);
    update(tempx);
    
    return tempx;
}

Rope::Node*Rope::rotateRight(Node *y)
{
if(y==nullptr||y->left==nullptr)return y;
    Node* tempy=y->left;
    Node* tempx=tempy->right;
    tempy->right=y;
    y->left=tempx;
    update(y);
    update(tempy);
    
    return tempy;
}

Rope::Node*Rope::rebalance(Node *node)
{
    if(node==nullptr)return node;
    update(node);
    int dis=height(node->left)-height(node->right);
    if(dis>1){
        if(height(node->left->left)<height(node->left->right))
        {node->left=rotateLeft(node->left);}
        return rotateRight(node);
    }
    if(dis<-1){
        if(height(node->right->right)<height(node->right->left))
        {node->right=rotateRight(node->right);}
        return rotateLeft(node);
    }
    update(node);
    return node;
}

void Rope::Rope::split(Node *node, int index, Node *&outLeft, Node *&outRight)
{
    outRight=nullptr;
    outLeft=nullptr;
    if(node==nullptr)return;
    if(node->isLeaf()){
        if(index>=node->weight){
            outLeft=new Node(node->data);
        }
        else{
            if(index<=0){
                outRight=new Node(node->data);
            }
            else{
                string sleft=node->data.substr(0,index);
                string sright= node->data.substr(index);
                if(sleft.empty()==false)outLeft= new Node(sleft);
                if(sright.empty()==false)outRight=new Node(sright);
            }
        }
    }else{ Node*root1=nullptr,*root2=nullptr;
        if(index<node->weight){
            split(node->left,index,root1,root2);
            outLeft=root1;
            outRight=rebalance(concatNodes(root2,node->right));
        }
        else{
            if(index>node->weight){
            split(node->right,index-node->weight,root1,root2);
            outLeft=rebalance(concatNodes(node->left,root1));
            outRight=root2;
        }else{
            outRight=node->right;
            outLeft=node->left;
        }
        }
        
    }
    return;
}

Rope::Node *Rope::concatNodes(Node *left, Node *right)
{
    if(left==nullptr)return right;
    if(right==nullptr)return left;
    Node* temp=new Node();
    temp->left=left;
    temp->right=right;
    update(temp);
    return rebalance(temp); 
}

char Rope::Rope::charAt(Node *node, int index) const
{
    if(node==nullptr)return '\0' ;
    if(node->isLeaf()){
        return node->data[index];
    }
    else{
        if(index<node->weight){
            return charAt(node->left,index);
        }else return charAt(node->right,index-node->weight);
    }
    return 0;
}

string Rope::toString(Node *node) const
{if(node==nullptr)return "";
if(node->isLeaf()){
    return node->data;
}else{return toString(node->left)+toString(node->right);};
}

void Rope::destroy(Node *&node){ 
    if(node==nullptr)return;
    destroy(node->left);
    destroy(node->right);
    delete node;
    node=nullptr;
}
int Rope::length() const
{
    return getTotalLength(root);
}
bool Rope::empty() const {
    return length() == 0;
}
char Rope::charAt(int index) const
{
    return charAt(root,index);
}


string Rope::substring(int start, int length) const
{   if(start<0||start+length>getTotalLength(root))throw out_of_range("Index is invalid!");
string a= toString(root);
    return a.substr(start,length);
}

Rope::Node**Rope:: splitChunks(const string& s, int& count) {
    count = (s.size() + CHUNK_SIZE - 1) / CHUNK_SIZE; 
    Node** chunks = new Node*[count];
    int idx = 0;
    for (int i = 0; i < count; i++) {
        string sub = s.substr(idx, CHUNK_SIZE);
        chunks[i] = new Node(sub);
        update(chunks[i]);
        idx += CHUNK_SIZE;
    }
    return chunks;
}
void Rope::insert(int index, const string &s)
{  
    if(index>getTotalLength(root)||index<0)throw out_of_range("Index is invalid!");
    if(s.empty())return;
    Node*temp=root;
        Node*root1=nullptr,*root2=nullptr;
        split(root,index,root1,root2);
        int count =0;
        Node* mid = nullptr;
    if (s.size() > CHUNK_SIZE) {
        int count;
        Node** chunks = splitChunks(s, count);

        mid = root1;
        for (int i = 0; i < count; i++) {
            mid = concatNodes(mid, chunks[i]);
        }
        delete[] chunks;
    } else {
        mid =concatNodes(root1, new Node(s));
        update(mid);
    }

    root = concatNodes(mid, root2);
    update(root);
    root = rebalance(root);
}

void Rope::deleteRange(int start, int length)
{   if(start<0||length<0||length+start>getTotalLength(root))throw out_of_range("Index is invalid!");
    Node*root1=nullptr,*root2=nullptr,*root3=nullptr,*root4=nullptr;
    split(root,start,root1,root2);
    split(root2,length,root3,root4);
    destroy(root2);
    root=concatNodes(root1,root4);
}

string Rope::toString() const
{
    return toString(root);
}

// ----------------- RopeTextBuffer -----------------
RopeTextBuffer::RopeTextBuffer() {
    // TODO
    cursorPos=0;
    history = new HistoryManager();
}

RopeTextBuffer::~RopeTextBuffer() {
    // TODO
    delete history;
}
void RopeTextBuffer::insert(const string& s){
    int bcur=cursorPos;
    rope.insert(cursorPos,s);
    cursorPos+=s.size();
    HistoryManager:: Action temp;
    temp.actionName="insert";
    temp.cursorBefore=bcur;
    temp.cursorAfter=cursorPos;
    temp.data=s;
    history->addAction(temp);

}
void RopeTextBuffer::deleteRange(int length){
    if(cursorPos+length-1>rope.length()){throw out_of_range("Length is invalid!");}
    string strdel = rope.substring(cursorPos, length);
    rope.deleteRange(cursorPos,length);
    HistoryManager:: Action temp;
    temp.actionName="delete";
    temp.cursorBefore=cursorPos;
    temp.cursorAfter=cursorPos;
    temp.data=strdel;
    history->addAction(temp);
}
void RopeTextBuffer::replace(int length, const string& s){
    if(cursorPos+length-1>rope.length()){
        throw out_of_range("Length is invalid!");
    }
    if(cursorPos>rope.length()||cursorPos<0)throw  out_of_range("cursorPos is invalid!");
    
    if(history->countre==0){
        history->curre=0;
    }else history->curre++;
    history->countre=history->curre+1;
        Rope *tempreold=new Rope[history->countre];

        for(int i=0;i<history->countre-1;i++){
        tempreold[i]=history->listold[i];
    }
        tempreold[history->curre]=rope;
        delete[]history->listold;
        history->listold=tempreold;
    
    string strrep=rope.substring(cursorPos,length);
    rope.deleteRange(cursorPos, length);
    rope.insert(cursorPos, s);
    HistoryManager:: Action temp;
    temp.actionName="replace";
    temp.cursorBefore=cursorPos;
    temp.cursorAfter=cursorPos+s.size();
    temp.data=strrep;
    history->addAction(temp);
    cursorPos+=s.size();
    //xu ly luu replace
    
    Rope *tempre=new Rope[history->countre];
    for(int i=0;i<history->countre-1;i++){
        tempre[i]=history->listnew[i];
    }
    tempre[history->curre]=rope;
    delete[] history->listnew;
    history->listnew=tempre;
}
void RopeTextBuffer::moveCursorTo(int index){
    if(index>rope.length()||index<0)throw out_of_range("Index is invalid!");
    
    HistoryManager:: Action temp;
    temp.actionName="move";
    temp.cursorBefore=cursorPos;
    temp.cursorAfter=index;
    temp.data="J";
    history->addAction(temp);
    
    cursorPos=index;
}
void RopeTextBuffer::moveCursorLeft(){
    if(cursorPos>rope.length()||cursorPos<1)throw cursor_error();
    HistoryManager:: Action temp;
    temp.actionName="move";
    temp.cursorBefore=cursorPos;
    temp.cursorAfter=cursorPos-1;
    temp.data="L";
    history->addAction(temp);
    
    cursorPos=cursorPos-1;
}
void RopeTextBuffer::moveCursorRight(){
    if(cursorPos>rope.length()-1||cursorPos<0)throw cursor_error();
    
    HistoryManager:: Action temp;
    temp.actionName="move";
    temp.cursorBefore=cursorPos;
    temp.cursorAfter=cursorPos+1;
    temp.data="R";
    history->addAction(temp);
    
    cursorPos=cursorPos+1;
}
int  RopeTextBuffer::getCursorPos() const{
    return cursorPos;
}
string RopeTextBuffer::getContent() const{
    return rope.toString();
}
int RopeTextBuffer::findFirst(char c) const{
    for(int i=0;i<rope.length();i++){if(rope.charAt(i)==c)return i;}
    return -1;
}
int* RopeTextBuffer::findAll(char c) const{
        int count=0;
        for(int i=0;i<rope.length();i++){if(rope.charAt(i)==c)count++;}
        int *temp=new int[count];
        int o=0;
        for(int i=0;i<rope.length();i++){if(rope.charAt(i)==c){temp[o]=i;o++;}}
        return temp;
}
void RopeTextBuffer::undo(){
    if(history->canUndo()==0)return;
    HistoryManager::Action a=history->listact[history->curpos];
    cout<<a.actionName<<endl;
    if (a.actionName == "insert") {
        cursorPos = a.cursorBefore;
        rope.deleteRange(cursorPos,a.data.size());
        cursorPos = a.cursorBefore;
    } else{ if (a.actionName == "delete") {
        cursorPos = a.cursorBefore;
        rope.insert(cursorPos, a.data);
        cursorPos = a.cursorBefore;
    } else{ if (a.actionName == "replace") {
        if(history->curcount>0){
        rope=history->listold[history->curre];
        history->curre--;
        cursorPos = a.cursorBefore;}
    } else{ if (a.actionName == "move") {
        cursorPos = a.cursorBefore;}}
    }}
    history->curpos--;
}
void RopeTextBuffer::redo(){
    if(history->canRedo()==0)return;
    history->curpos++;
    HistoryManager::Action a=history->listact[history->curpos];
    if (a.actionName == "insert") {
        cursorPos = a.cursorBefore;
        rope.insert(cursorPos, a.data);
        cursorPos = a.cursorAfter;
    } else if (a.actionName == "delete") {
        cursorPos = a.cursorBefore;
        rope.deleteRange(cursorPos,a.data.size());
      cursorPos = a.cursorAfter;
    } else if (a.actionName == "replace") {
        if(history->curre<=history->countre-1){
        history->curre++;
        rope=history->listnew[history->curre];
        cursorPos = a.cursorAfter;}
    } else if (a.actionName == "move") {
        cursorPos = a.cursorAfter;
    }
}
void RopeTextBuffer::printHistory() const{
    history->printHistory();
}
// TODO: implement other methods of TextBuffer

// ----------------- HistoryManager -----------------
RopeTextBuffer::HistoryManager::HistoryManager() {
    // TODO
    listnew=nullptr;
    listold=nullptr;
    listact=nullptr;
    curcount=0;
    curpos=0;
    countre=0;
    curre=0;
}

RopeTextBuffer::HistoryManager::~HistoryManager() {
    delete[] listnew;
    delete[] listold;
    delete[]listact;
    curpos=0;
    curcount=0;
    countre=0;
    curre=0;
}
void RopeTextBuffer::HistoryManager::addAction(const Action& a){
    if(curcount==0){curpos=0;}
    else curpos++;
    curcount=curpos+1;
    Action* temp=new Action[curcount];
    for(int i=0;i<curpos;i++){
        temp[i]=listact[i];
    }
    temp[curcount-1]=a;
    delete[] listact;
    listact=temp;
    return;
}
bool RopeTextBuffer::HistoryManager::canUndo() const{
    if(curpos>0&&curcount>0)return true;
    else return false;
}
bool RopeTextBuffer::HistoryManager::canRedo() const{
    if(curcount-1>curpos)return 1;
    return 0;
}
void RopeTextBuffer::HistoryManager::printHistory() const{
    cout<<"[";
    Action *a=listact;
    for(int i=0;i<curcount;i++){
         cout << "(" << a[i].actionName << ", " << a[i].cursorBefore << ", " << a[i].cursorAfter << ", " << a[i].data << ")";
        if (i + 1 < curcount) cout << ", ";
    }
    cout<<"]";
}
void RopeTextBuffer::HistoryManager::clear(){
    this->~HistoryManager();
    new (this) HistoryManager();
}
//TODO: implement other methods of HistoryManager

