Assignment 7 (Corpus Q and A)

For Part 2, 

files required : Node.h, Node.cpp, dict.h, dict.cpp, search.h, search.cpp, qna_tool.h, qna_tool.cpp, api_call.py, api_call_palm.py, api_hugging_chat.py, deploy_code.cpp

Run the following command on the terminal : 
For LINUX : g++ Node.h Node.cpp dict.h dict.cpp search.h search.cpp qna_tool.h qna_tool.cpp deploy_code.cpp -std=c++11
For Windows : g++ Node.h Node.cpp dict.h dict.cpp search.h search.cpp qna_tool.h qna_tool.cpp deploy_code.cpp -std=c++11
(the -std=c++11 flag ignores the warnings of #pragma once from being displayed to the user)
The above command compiles all the required files into an executable by the name main.out for LINUX and main.exe for Windows

To execute the created files :
For LINUX : ./a.out
For Windows : ./a.exe

In the terminal, first enter the number of books to be analysed by the program.
Then enter the question you want answered from the corpus of the books fed to the program.

NOTE : the program requires that all the files in the corpus follow a set pattern of naming so as to be able to access them independently without assitance from user.
Currently, the program is written assuming that all the books of the corpus are written in the format, "mahatma-gandhi-collected-works-volume-<Book_Code>.txt"

The program is already fed with API_KEYS and/or login credentials for all the APIS, ChatGPT, HuggingChat, Google PaLM API

The program then determines the top k (currently set to 5) paragraphs relevant to the the question which is then fed to ChatGPT and PaLM and then both their outputs is given to HuggingChatr which generates the final output answer displayed to the user.
(Since the program makes use of APIs and it needs to connect to the internet and retreive data so it takes some time for program to complete execution.)

---> The program generates files, query.txt which contains the topk paragraphs determined with the question asked, which in its entirety is asked to the API
                                  hug_query.txt, the text that is asked to HuggingChat to retreive the final output, contains the answers from ChatGPT and PaLM