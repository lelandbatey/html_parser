#ifndef HTML_LEXER_H
#define HTML_LEXER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


class HtmlLexer
{
public:
    HtmlLexer();
    HtmlLexer(string& input_document);

    void tokenize(string& input_document);
    string get_next_token();
    vector<string> get_tokens();
    bool eof();

private:
    vector<string> _tokens;
    unsigned int _current_token_index;
    bool _eof;
};



#endif