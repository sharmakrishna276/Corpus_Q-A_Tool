#include "dict.h"

void lowercase(string &word) {
    int len = word.size();
    for (int i=0;i<len;i++) {
        if (word[i]>='A' && word[i]<='Z') {
            word[i]+=32;
        }
    }
}

int hashvalue(string id, int size) {
    if(id == "")
    {
        return 0;
    }
    int x = 71;
    int a = 9901;
    int b = 8101;
    int h = 0;
    int n = id.size();
    int m = size;
    for(int i = 0; i < n;i++)
    {
        h = (h*x)%m;
        h += int(id[i]);
        h %= m;
    }
    int ans = (a*h)%m;
    ans = (ans + b)%m;
    ans = abs(ans);
    return ans;
}

Dict::Dict()
{
    size = 97;
    for(int i = 0; i < 35;i++)
    {
        pref[i] = -1;
    }
    for(int i = 35;i < 128;i++)
    {
        pref[i] = 0;
    }
    pref[39] = -1;
    pref[40] = -1;
    pref[41] = -1;
    pref[44] = -1;
    pref[45] = -1;
    pref[46] = -1;
    pref[58] = -1;
    pref[59] = -1;
    pref[63] = -1;
    pref[64] = -1;
    pref[91] = -1;
    pref[93] = -1;
    pref[127] = -1;
    hashtable.reserve(size);
    for(int i = 0; i < size;i++)
    {
        hashtable[i] = new SymbolTable();
    }
}
Dict::Dict(int sz){
    size=sz;
    for(int i = 0; i < 35;i++)
    {
        pref[i] = -1;
    }
    for(int i = 35;i < 128;i++)
    {
        pref[i] = 0;
    }
    pref[39] = -1;
    pref[40] = -1;
    pref[41] = -1;
    pref[44] = -1;
    pref[45] = -1;
    pref[46] = -1;
    pref[58] = -1;
    pref[59] = -1;
    pref[63] = -1;
    pref[64] = -1;
    pref[91] = -1;
    pref[93] = -1;
    pref[127] = -1;
    hashtable.reserve(size);
    for(int i = 0; i < size;i++)
    {
        hashtable[i] = new SymbolTable();
    }
}

Dict::~Dict(){
    for (int i=0;i<size;i++) {
        delete hashtable[i];
    }
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    string curr = "";
    for(char i : sentence)
    {
        int a = tolower(i);
        i = a;
        if(pref[i] == -1)
        {
            if(curr != "")
            {
                int hash_val = hashvalue(curr,size);
                hashtable[hash_val]->insert(curr,1);
            }
            curr = "";
        }
        else
        {
            curr += i;
        }
    }
    if(curr != "")
    {
        int hash_val = hashvalue(curr,size);
        hashtable[hash_val]->insert(curr,1);
    }
    return;
}

void Dict::insert_word(string word,long long count)
{
    int hash_val = hashvalue(word,size);
    hashtable[hash_val]->insert(word,count);
}

long long Dict::get_word_count(string word){
    lowercase(word);
    string lower_word = word;
    int idx=hashvalue(lower_word,size);
    SymNode* ptr = hashtable[idx]->search(word);
    if(ptr == NULL)
    {
        return 0;
    }
    return ptr->address;
}

void call(fstream &fh, Dict::SymNode* node){
    fh << node->key << ", " << node->address <<"\n";
    if (node->left != NULL){
        call(fh, node->left);
    }
    if (node->right != NULL){
        call(fh, node->right);
    }
}

void Dict::dump_dictionary(string filename){
    // Implement your function here
    fstream file_handle;
    file_handle.open(filename, std::ios::out);
    for (int i = 0; i < size; i++){
        if (hashtable[i]->root != NULL){
            call(file_handle, hashtable[i]->root);
        }
    }
    file_handle.close();
    return;
}
