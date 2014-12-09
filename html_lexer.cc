#include <iostream>
#include <vector>
#include <string>

#include "html_lexer.h"

using namespace std;


HtmlLexer::HtmlLexer(): _current_token_index(0), _eof(false){}
HtmlLexer::HtmlLexer(string& input_document): _current_token_index(0), _eof(false){ tokenize(input_document); }


void HtmlLexer::tokenize(string& input_document){
    vector<int> breaks;

    for (unsigned int i = 0; i < input_document.length(); ++i){
        if (input_document[i] == '<'){
            if (!breaks.size()){
                breaks.push_back(i);
            // Special case for tokenizing comments and all their contents
            } else if (input_document.find("<!--") == i){
                if (breaks.back() != i){
                    breaks.push_back(i);
                }
                unsigned int end_comment = input_document.find("-->");
                if (end_comment != std::string::npos){
                    i = end_comment;
                }
            // Special case for tokenizing script tag and it's entire contents
            } else if (input_document.find("<script") == i){
                if (breaks.back() != i){
                    breaks.push_back(i);
                }
                // Causes for loop to pick up the ">" of the closing script
                // tag as the end of the opening.
                unsigned int end_script = input_document.find("/script");
                if (end_script != std::string::npos){
                    i = end_script;
                }
            } 
            else if (breaks.back() != i){
                breaks.push_back(i);
            }
        } else if (input_document[i] == '>'){
            // We add one to make substrings simple and easy
            breaks.push_back(i+1);
        }
    }

    for (unsigned int i = 0; i < breaks.size(); ++i){
        if (i){
            int prior = breaks[i-1];
            int substring_length = breaks[i] - prior;
            string tmp = input_document.substr(prior, substring_length);
            _tokens.push_back(tmp);
        }
    }
}


string HtmlLexer::get_next_token(){
    string to_return;
    if (!this->eof()){
        to_return = _tokens[_current_token_index];
        ++_current_token_index;
    } else {
        to_return = string('\0');
    }
    return to_return;
}

vector<string> HtmlLexer::get_tokens(){
    return _tokens;
}

bool HtmlLexer::eof(){
    if (_current_token_index == _tokens.size()){
        _eof = true;
    }
    return _eof;
}
