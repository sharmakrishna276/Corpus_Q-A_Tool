#include <assert.h>
#include <sstream>
#include "qna_tool.h"
#include <cmath>

using namespace std;

void readfile (Dict& d) {
    ifstream file("unigram_freq.csv");
    if (!file.is_open()) {
        cout << "Error opening the file!" << endl;
        return;
    }
    string line;
    getline(file,line);
    while (getline(file, line)) {
        istringstream ss(line);
        vector<string> row;
        string cell;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        long long temp_count=stoll(row[1]);
        d.insert_word(row[0],temp_count);
    }
    file.close();
}

class my_para {
    public:
    long double para_score = 0.0;
    Dict my_dict;
    int bk;
    int pgn;
    int prg;
    my_para()
    {
        para_score = 0.0;
        my_dict = Dict();
        int bk = -1;
        int pgn = -1;
        int prg = -1;
    }
};

QNA_tool::QNA_tool(){
    // Implement your function here
    book=-1;
    para_count=0;
    para_temp=-1;
    page_temp=-1;
    // readfile(gen_corpus);
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
}

QNA_tool::~QNA_tool(){
    // Implement your function here
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    corpus.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    if (book==book_code && page_temp==page && para_temp==paragraph) {
        para[para_count-1]->my_dict.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    }
    else {
        my_para* new_para = new my_para();
        new_para->my_dict.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
        new_para->bk=book_code;
        new_para->pgn=page;
        new_para->prg=paragraph;
        para.push_back(new_para);
        para_count++;
        book=book_code;
        page_temp=page;
        para_temp=paragraph;
    }
    return;
}

void splitsentence(vector<string> &words,string sentence,int* pref) {
    string word="";
    int len = sentence.size();
    for (int i=0;i<len;i++) {
        char x = sentence[i];
        if (pref[x] == -1) {
            int num = word.size();
            if (num>0) {
                words.push_back(word);
            }
            word="";
        }
        else {
            word+=x;
        }
    }
    int num = word.size();
    if (num>0) {
        words.push_back(word);
    }
}

int parent(int idx)
{
    return (idx-1)/2;
}

void hup(vector<my_para*>& heap, int shize)
{
    int child = shize-1;
    int par = parent(child);
    while(par != child && heap[par]->para_score > heap[child]->para_score)
    {
        my_para* tempu = heap[par];
        heap[par] = heap[child];
        heap[child] = tempu;
        child = par;
        par = parent(child);
    }
}

void hdown(vector<my_para*>& heap, int shize)
{
    int par = 0;
    while(2*par + 1 < shize)
    {
        int l = 2*par+1;
        int r = 2*par + 2;
        my_para* mini = heap[l];
        if(r < shize)
        {
            if(heap[r]->para_score < mini->para_score)
            {
                mini = heap[r];
            }
        }
        if(mini->para_score < heap[par]->para_score)
        {
            if(mini->para_score == heap[l]->para_score)
            {
                heap[l] = heap[par];
                heap[par] = mini;
                par = l;
            }
            else
            {
                heap[r] = heap[par];
                heap[par] = mini;
                par = r;
            }
        }
        else
        {
            break;
        }
    }
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    vector<string> words;
    splitsentence(words,question,pref);
    int sz=words.size();
    vector<long long> npar;
    for(int i = 0; i < sz;i++)
    {
        long long thu = 0;
        npar.push_back(thu);
        for(int j = 0; j < para_count;j++)
        {
            if(para[j]->my_dict.get_word_count(words[i]) != 0)
            {
                npar[i]++;
            }
        }
    }
    // vector<string> vec = {"in","at","the","is","am","are","was","were","he","she","they","them","has","had","have","on","of","for","into","that","which","as","like","such","a","an","those","these","their","to","with","by","from","but","or","so","yet","than","about", "i","what","why","who","whom","where","when"};
    vector<string> vec = {"the", "of", "and", "to", "a", "in", "for", "is", "on", "that", "by", "this", "with", "i", "you", "it", "not", "or", "be", "are", "from", "at", "as", "your", "all"};
    for (int j = 0; j < para_count; j++) {
        for (int i=0;i<sz;i++) {
            long double score=0;
            bool c = 1;
            for (string w:vec){
                if (words[i] == w){
                    c = 0;
                    break;
                }
            }
            if (c == 0){
                continue;
            }
            // if (gen_corpus.get_word_count(words[i]) >= 2000000000){
            //     continue;
            // }
            long long freq = para[j]->my_dict.get_word_count(words[i]);
            long double half = 0.5;
            long double one = 1;
            score = ((freq)/(freq+half+one)+one)*logl((para_count-npar[i]+half)/(npar[i]+half) + one);
            para[j]->para_score+=(score);
        }
        if(j < k)
        {
            hup(para,j+1);
        }
        else if(para[j]->para_score > para[0]->para_score)
        {
            my_para* tp = para[j];
            para[j] = para[0];
            para[0] = tp;
            hdown(para,k);
        }
    }
    //sort(para.rbegin(),para.rend(),my_compare);
    Node* head = new Node(para[0]->bk,para[0]->pgn,para[0]->prg,-5,-5);
    int shize = k;
    my_para* tomp = para[0];
    para[0] = para[shize-1];
    para[shize-1] = tomp;
    shize--;
    hdown(para,shize);
    head->left=nullptr;
    head->right=nullptr;
    for (int i=1;i<k;i++) {
        Node* temp = new Node(para[0]->bk,para[0]->pgn,para[0]->prg,-5,-5);
        my_para* tomp = para[0];
        para[0] = para[shize-1];
        para[shize-1] = tomp;
        shize--;
        hdown(para,shize);
        temp->left=nullptr;
        temp->right=head;
        head->left=temp;
        head=temp;
    }
    return head;
}

void QNA_tool::query(string question, string filename1, string filename2, string one_more_file, string filename3){
    // Implement your function here
    Node* t = QNA_tool::get_top_k_para(question, 5);
    query_llm(filename1, filename2, one_more_file, t, 5, "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", "AIzaSyCC-Fe72-Ibmd8_ArIEQHdfiZnNfJM2WK8", question, filename3, "example12768@gmail.com", "COL106_hugchat");
    //std::cout << "Q: " << question << std::endl;
    //std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename1, string filename2, string one_more_file, Node* root, int k, string API_KEY_GPT, string API_KEY_PALM, string question, string filename3, string email, string pswd){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "On the basis of this, ";
    outfile << question;
    outfile <<"\n\n";
    outfile << "\nI want answers strictly based on the excerpts provided.\nRead each excerpt line by line and tell me the relevant information from excerpts based on the question that I provided!";
    // outfile << "\nUse only contextual information. Do not use your own knowledge database or the internet. Don't explain to me the modifications done. Simply state the answer.";
    outfile << "\nIt is not neccesary to use all the five excerpts provided to answer the question. I want you to use only the most relevant information in the excerpt to anwer the question.";
    outfile << "\nDO NOT PROVIDE ANY NOTES, or your opinions about the answer of the question. I want you to just output the answer. DO NOT USE INTERNET OR OTHER DATABASES as source and only use the pargraphs i give you as source";
    outfile << "\nTemperature = 0.4";
    outfile << "\nTop_p = 0.5";
    outfile << "\nFrequency penalty = 0.6";
    outfile << "\nPresence penalty = 0.1";
    outfile << "\nStopsequence = \"\\n\"";
    outfile << "\nVivid = 1";
    // outfile <<"\nmax_length = 100";
    //outfile << "\nAlso state where exactly in the excerpt did the line that you stated occurs";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself
    ofstream fhandle(one_more_file, std::ios::out);
    fhandle << "I want to answer the question, ";
    fhandle << question;
    // fhandle <<  "i have two possible answers for it:\n";
    // fhandle << "a) ";
    fhandle.close();
    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename1;
    command += " ";
    command += API_KEY_GPT;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";
    command += " ";
    command += one_more_file;

    system(command.c_str());
    // ofstream fhandle1(one_more_file, std::ios::app);
    // fhandle1 << "\nb) ";
    // fhandle1.close();
    string cmd2 = "python3 ";
    ofstream out("query.txt", std::ios::out);
    out << "The query is : ";
    out << question;
    out << "\n I have given you 5 paragraphs, I want you to answer my query. DO NOT OUTPUT ANY OTHER INFORMATION, YOUR OWN OPINION. If no line is relevant to the query, output \" no relevant information \". YOU ARE NOT PERMITTED TO USE INTERNET SEARCH OR OTHER DATABASES as a source and use only the 5 paragraphs I give you as source";
    out.close();
    cmd2 += filename2;
    cmd2 += " ";
    cmd2 += API_KEY_PALM;
    cmd2 += " ";
    cmd2 += to_string(k);
    cmd2 += " ";
    cmd2 += "query.txt";
    cmd2 += " ";
    cmd2 += one_more_file;

    system(cmd2.c_str());
    ofstream fhandle2(one_more_file,std::ios::app);
    fhandle2 << "\nwithout using any other knowledge and just based on these answers, construct the most relevant answer i want you to take the BEST OF THE TWO ANSWERS if possible and NOT JUST SELECT ONE of them as being more relevant than the other. And just output the final answer and NO OTHER REMARKS/COMMENTS/NOTES of your own.";
    // fhandle2 << "Read each excerpt line by line and tell me the relevant information from excerpts based on the question that I provided! It is not neccesary to use all the them provided to answer the question. I want you to use only the most relevant information to anwer the question. Don't give any introductions like, \"based on the inforemation provided, the most relevant answer is..\". Just output the answer.";
    // fhandle2 << "\nDONT WRITE \"Sure, here is the answer...\". Just give me the answer, and don't give me the points that are most relevant. Use them to create a properly structured answer which will be the only output you provide me.";
    fhandle2 << "\nTemperature = 0.7";
    fhandle2 << "\nTop_p = 1.0";
    fhandle2 << "\nFrequency penalty = 0.6";
    fhandle2 << "\nPresence penalty = 0.1";
    fhandle2 << "\nStopsequence = \"\\n\"";
    fhandle2 << "\nGive more priority to technical information and data over sentence structure when comparing points. I want you to output the final answer combining the most relevant content and points from both answers and not their grammar. DO NOT SAY \"Sure, here's the combined answer based on given parameters.\" DO NOT USE INTERNET OR ANY OTTHER DATABASE TO ANSWER THE QUESTION. ONLY USE THE INPUT PROVIDED";
    fhandle2.close();

    string cmd1 = "python3 ";
    cmd1 += filename3;
    cmd1 += " ";
    cmd1 += email;
    cmd1 += " ";
    cmd1 += pswd;
    cmd1 += " ";
    cmd1 += one_more_file;

    system(cmd1.c_str());
    return;
}
