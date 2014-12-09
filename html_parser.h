#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <iostream>
#include <vector>
#include <string>

#include "xml_node.cc"
#include "html_lexer.cc"

using namespace std;


class HtmlParser
{
public:
    HtmlParser();
    HtmlParser(string& input_document);
    
    void parse(string& input_document);
    void print();
    void recursive_tree_printer(XmlNode* node);

    vector<string> find_all_attributes(string attribute_name);


private:
    HtmlLexer _lexer;
    XmlNode _root;
};

string get_type(string token);

bool is_opening_tag(string token);
bool is_closing_tag(string token);
bool is_self_closing_tag(string token);
bool is_comment_tag(string token);
bool is_doctype_tag(string token);
bool is_script_tag(string token);
bool is_text_tag(string token);


#endif