// Do NOT add any other includes
#include "search.h"

Sentence::Sentence(){
    book_code=0;
    page=0;
    paragraph=0;
    sentence_no=0;
    text="";
}

Sentence::Sentence(int b_code, int pg, int para, int s_no, string txt){
    book_code = b_code;
    page = pg;
    paragraph = para;
    sentence_no = s_no;
    text = txt;
}

Sentence::~Sentence(){

}

SearchEngine::SearchEngine(){
    // Implement your function here  
    store.clear();
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
    store.clear();
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    make_lower(sentence); 
    Sentence temp = Sentence(book_code, page, paragraph, sentence_no, sentence);
    store.push_back(temp);
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here  
    Node* head = nullptr; Node* ptr;
    n_matches=0;

    for (Sentence s : store){
        vector<int> offsets;
        this->KMPallmatches(offsets ,s.text , pattern);
        int size = offsets.size();
        for (int i = 0;i < size; i++){
            Node* temp = new Node(s.book_code,s.page,s.paragraph, s.sentence_no, offsets[i]);
            temp->right = nullptr;
            // insertion at end
            if (!head){
                head = temp;
                head->left = head->right = nullptr;
                // n_matches++;
                ptr = head;
                continue;
            }
            ptr->right = temp;
            temp->left = ptr;
            ptr = ptr->right;
            // n_matches++;
        }
        n_matches = size;
    }

    return head;
}

void SearchEngine::make_lower(string &s){
    int num = s.length();
    for (int i=0;i<num;i++){
        if (s[i]>='A' and s[i]<='Z'){
            s[i] = s[i]+32;
        }
    }
}

void SearchEngine::KMPallmatches(vector<int> &ans, string &text, string &pattern){
    make_lower(pattern);
    int text_length = text.length();
    int pattern_length = pattern.length();

    // getting lps

    int i = 1, j = 0; // two pointers, i represents current character 
    int n = pattern.size(); //size of pattern
    vector<int> lps(n);  //lps array
    lps[0] = 0; //lps[0] is always zero, since empty string is only proper prefix

    for (i = 1; i < n; i++){
        if (pattern[j] == pattern[i]){  //matching continues
            lps[i] = ++j;  //substring from 0 to j (j+1 characters) matches the substring from i-j to i, lps[i]=j+1
        }  
        else{                         //matching stops
            if (j > 0) {                 
                j = lps[j-1]; i--;      // current character remains same, looking for a shorter substring
                // sending j backwards to search in shorter substrings, since matching stopped at current j
            }
            else lps[i] = 0;             // pattern[0]!=pattern[i], and lps[i] is less than 1, so lps[i]=0
        }
    }

    // lps obtained

    i=0,j=0;
    
    while (i<text_length){
        if (pattern[j]==text[i]){
            if (j==pattern_length-1)  ans.push_back(i-pattern_length+1);
            i++; j++;    // keep sliding j till entire pattern is covered
        }
        else if (j>0){   // adjusting j according to the number of chars that can be skipped
            j = lps[j-1];
        }
        else{      // if j=0, then simply increase i
            i++;
        }
    }
}