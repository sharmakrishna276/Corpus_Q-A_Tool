// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
class LLNode;
class Dict {
private:
    // You can add attributes/helper functions here

public:
class SymNode {
public:
    string key;
    int height;
    long long address=1;

    SymNode* par;
    SymNode* left;
    SymNode* right;
SymNode(string k)
{
    this->key = k;
    this->height = 0;
    this->address = 1;
    this->par = NULL;
    this->left = NULL;
    this->right = NULL;
}

~SymNode()
{
    if (this->left != NULL)
    {
        delete this->left;
    }
    if (this->right != NULL)
    {
        delete this->right;
    }
}
};
class SymbolTable {
public:
    SymNode* root = NULL;

SymbolTable(){
    this->root = NULL;
}

int h(SymNode* node)
{
    if(node == NULL)
    {
        return -1;
    }
    return node->height;
}

SymNode* Left(SymNode* root, SymNode* ptr)
{
    SymNode* temp = ptr->right;
    temp->par = ptr->par;
    ptr->right = temp->left;
    if(ptr->right != NULL)
    {
        ptr->right->par = ptr;
    }
    temp->left = ptr;
    if(temp->par == NULL)
    {
        root = temp;
    }
    else
    {
        if(ptr->par->left == ptr)
        {
            ptr->par->left = temp;
        }
        else
        {
            ptr->par->right = temp;
        }
    }
    ptr->par = temp;
    ptr->height = 1 + std::max(h(ptr->right),h(ptr->left));
    temp->height = 1 + std::max(h(temp->right),h(temp->left));
    return root;
}

SymNode* Right(SymNode* root,SymNode* ptr)
{
    SymNode* temp = ptr->left;
    temp->par = ptr->par;
    ptr->left = temp->right;
    if(ptr->left != NULL)
    {
        ptr->left->par = ptr;
    }
    temp->right = ptr;
    if(temp->par == NULL)
    {
        root = temp;
    }
    else
    {
        if(ptr->par->left == ptr)
        {
            ptr->par->left = temp;
        }
        else
        {
            ptr->par->right = temp;
        }
    }
    ptr->par = temp;
    ptr->height = 1 + std::max(h(ptr->right),h(ptr->left));
    temp->height = 1 + std::max(h(temp->right),h(temp->left));
    return root;
}

void insert(string k,long long cnt){
    if(root == NULL)
    {
        SymNode* ins = new SymNode(k);
        ins->address = cnt;
        root = ins;
        return;
    }
    SymNode* parent = NULL;
    SymNode* ptr = root;
    while(ptr != NULL)
    {
        if(ptr->key > k)
        {
            parent = ptr;
            ptr = ptr->left;
        }
        else if(ptr->key < k)
        {
            parent = ptr;
            ptr = ptr->right;
        }
        else
        {
            ptr->address+= cnt;
            return;
        }
    }
    SymNode* ins = new SymNode(k);
    ins->address = cnt;
    if(parent->key > k)
    {
        parent->left = ins;
        ins->par = parent;
    }
    else
    {
        parent->right = ins;
        ins->par = parent;
    }
    while(parent != NULL)
    {
        parent->height = 1 + std::max(h(parent->left),h(parent->right));
        if(h(parent->left) > 1 + h(parent->right))
        {
            if(h(parent->left->right) > h(parent->left->left))
            {
                this->root = Left(this->root,parent->left);
            }
            this->root = Right(this->root,parent);
        }
        else if(h(parent->right) > 1 + h(parent->left))
        {
            if(h(parent->right->right) < h(parent->right->left))
            {
                this->root=Right(this->root,parent->right);
            }
            this->root = Left(this->root,parent);
        }
        parent = parent->par;
    }
    return;
}

SymNode* search(string k){
    SymNode* ptr = this->root;
    while(ptr != NULL)
    {
        if(ptr->key > k)
        {
            ptr = ptr->left;
        }
        else if(ptr->key < k)
        {
            ptr = ptr->right;
        }
        else
        {
            return ptr;
        }
    }
    return NULL;
}

~SymbolTable(){
    if(this->root != NULL)
    {
        delete this->root;
    }
}
};
    vector<SymbolTable*> hashtable;
    int size;
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    int pref[128];
    Dict(int sz);
    Dict();
    ~Dict();
    void insert_word(string word, long long count);

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};
