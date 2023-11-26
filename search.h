// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

class Sentence{
    public:
    int book_code, page, paragraph, sentence_no;
    string text;

    Sentence();

    Sentence(int b_code, int pg, int para, int s_no, string txt);

    ~Sentence();
};

class SearchEngine {
private:
    // You can add attributes/helper functions here
    vector<Sentence> store;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/

    void make_lower(string &s);
    void KMPallmatches(vector<int> &ans, string &text, string &pattern);

};