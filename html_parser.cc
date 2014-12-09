#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "html_parser.h"
#include "lazy_string.h"

#ifndef DEBUG
#define DEBUG 0
#endif

using namespace std;

HtmlParser::HtmlParser(): _root(string("root")){}
HtmlParser::HtmlParser(string& input_document): _root(string("root")){
    this->parse(input_document);
}


// Returns the type of a token representing an opening or closing tag.
string get_type(string token){
    string type("");
    int offset;
    if (is_opening_tag(token)){
        offset = 1;
    } else if (is_closing_tag(token)){
        offset = 2;
    }

    for (int i = offset; i < token.size(); ++i){
        if (token[i] != ' ' && token[i] != '>'){
            type.push_back(token[i]);
        } else {
            break;
        }
    }
    return type;
}


// Returns true if the passed in token represents an opening tag. Examples of
// opening tags are:
//     "<a>", "<html>", "<div>"
//
// If the tag is a closing tag or any other kind of tag, this returns false.
bool is_opening_tag(string token){
    if (token.length() > 2){
        if (token[0] == '<' && token[1] != '/'){
            return true;
        }
    }
    return false;
}
// Returns true if the passed in token represents a closing tag. Examples of
// closing tags are:
//     "</a>", "</body>", "</div>"
//
// If the tag is a closing tag or any other kind of tag, this returns false.
bool is_closing_tag(string token){
    if (token.length() > 3){
        if (token[0] == '<' && token[1] == '/'){
            return true;
        }
    }
    return false;
}
bool is_self_closing_tag(string token){
    if (token.length() > 3){
        if (is_opening_tag(token) && token[token.size()-2] == '/'){
            return true;
        } else if (get_type(token) == "link"){
            return true;
        }
    }
    return false;
}
bool is_comment_tag(string token){
    if (token.find("<!--") == 0){
        return true;
    }
    return false;
}
bool is_doctype_tag(string token){
    if (!is_comment_tag(token) && token[1] == '!'){
        return true;
    }
    return false;
}
bool is_script_tag(string token){
    if (token.find("<script") == 0){
        return true;
    }
    return false;
}
bool is_text_tag(string token){
    if (token.find_first_of("<>") == std::string::npos){
        return true;
    }
    return false;
}

string clean_tag(string token){
    int trim, start;
    if (is_closing_tag(token)){
        start = 2; trim = 3;
    } else if (is_self_closing_tag(token)){
        start = 1; trim = 3;
    } else if (is_opening_tag(token)){
        start = 1; trim = 2;
    }

    string trimmed = token.substr(start, token.size()-trim);
    return trimmed;
}

string remove_quotes(string quoted_str){
    // Assumes that the string we've been given begins and ends with quotes.
    return quoted_str.substr(1, quoted_str.size()-2);
}


vector<string> tokenize_opening_tags(string opening_tag){
    opening_tag = clean_tag(opening_tag);
    vector<string> tokens;
    
    bool in_quotes = false, escaped = false;
    string buffer("");
    char delim = 0;

    for (int i = 0; i < opening_tag.size(); ++i){
        char cc = opening_tag[i];
        if (!in_quotes && (cc == ' ' || cc == '>' || cc == '/')){
            // Important that these two if statements be separate. If buffer
            // has no size, then we do nothing, which is intended behaviour.
            if (buffer.size()){
                tokens.push_back(buffer);
                buffer = string("");
            }
        } else if (!in_quotes && cc == '='){
            // Tokenizes equals symbol as it's own token
            if (buffer.size()){
                tokens.push_back(buffer);
                tokens.push_back(string("="));
                buffer = string("");
            }
        } else {
            buffer.push_back(cc);
            escaped = false;
            if (!in_quotes && (cc == '"' || cc == '\'')){
                delim = cc;
                in_quotes = true;
            } else if (in_quotes && !escaped && cc == delim){
                in_quotes = false;
            } else if (!escaped && cc == '\\'){
                escaped = true;
            }
        }
    }
    if (buffer.size()){
        tokens.push_back(buffer);
    }

    return tokens;
}

map<string, string> get_attributes(string token){
    map<string, string> attributes;

    vector<string> attr_tokens = tokenize_opening_tags(token);
    string temp_key(""), temp_value("");
    

    if (attr_tokens.size() < 2){
        return attributes;
    }

    for (int i = 1; i < attr_tokens.size(); ++i){
        string temp_key = attr_tokens[i];
        string temp_value = "";

        if (i+2 < attr_tokens.size()){
            if (attr_tokens[i+1] == "="){
                temp_value = attr_tokens[i+2];
                i = i+2;
                if (temp_value.find_first_of("'\"") != std::string::npos){
                    temp_value = remove_quotes(temp_value);
                }
            }
        }
        attributes[temp_key] = temp_value;
    }
    return attributes;
}


void HtmlParser::parse(string& input_document){
    map<string, string> attributes;
    _lexer = HtmlLexer(input_document);
    XmlNode* root = &_root;

    XmlNode* current_node = root;
    string type("");

    while (!_lexer.eof()){
        string token = _lexer.get_next_token();

        if (DEBUG){
            std::cerr << token << std::endl;
        }

        if (is_comment_tag(token)){
            new XmlNode(current_node, "comment", token);

        } else if (is_doctype_tag(token)){
            new XmlNode(current_node, "DOCTYPE", token);

        } else if (is_script_tag(token)){
            new XmlNode(current_node, "script", token);
        
        } else if (is_opening_tag(token)){
            type = get_type(token);
            attributes = get_attributes(token);
            current_node = new XmlNode(current_node, type, attributes);
            if (is_self_closing_tag(token)){
                current_node = current_node->get_parent();
            }
        
        } else if (is_closing_tag(token)){
            type = get_type(token);
            XmlNode* tn = current_node;
            // if this closing tag doesn't coorespond to an opening tag.
            while (tn->get_type() != type){
                if (DEBUG){
                    std::cout << tn->get_type() << std::endl;
                }
                if (tn->is_root()){
                    if (DEBUG){
                        std::cerr << "'" << token << "'" << std::endl;
                    }
                    std::cerr << "HTML is malformed." << std::endl;
                    return;
                }
                tn = tn->get_parent();
            }
            current_node = tn->get_parent();
        } else if (is_text_tag(token)){
            new XmlNode(current_node, "text", token);
        }
        type = "";
        attributes.clear();
    }
}

// Recursively prints the correct structure of an HTML tree.
void HtmlParser::recursive_tree_printer(XmlNode* node){
    
    for (int i = 0; i < node->get_depth(); ++i){
        std::cout << "    ";
    }
    if (node->has_text()){
        std::cout << node->get_text() << std::endl;
    } else {
        std::cout << "<" << node->get_type() << ">" << std::endl;
    }

    if (node->has_children()){
        vector<XmlNode*> children = node->get_children();
        for (unsigned int i = 0; i < children.size(); ++i){
            recursive_tree_printer(children[i]);
        }
    }

    if (!node->has_text()){
        for (int i = 0; i < node->get_depth(); ++i){
            std::cout << "    ";
        }
        std::cout << "</" << node->get_type() << ">" << std::endl;
    }
}

void HtmlParser::print(){
    if (_root.has_children()){
        vector<XmlNode*> children = _root.get_children();
        for (unsigned int i = 0; i < children.size(); ++i){
            recursive_tree_printer(children[i]);
        }
    }
}

void find_attrs(string attr_val, XmlNode* node, vector<string>* found){

    if (node->has_attribute(attr_val)){
        found->push_back(node->get_attribute(attr_val));
    }

    if (node->has_children()){
        vector<XmlNode*> children = node->get_children();
        for (unsigned int i = 0; i < children.size(); ++i){
            find_attrs(attr_val, children[i], found);
        }
    }
}

// Trampolines into the "find_attrs" function
vector<string> HtmlParser::find_all_attributes(string attribute_name){
    vector<string> found_attributes;
    find_attrs(attribute_name, &_root, &found_attributes);
    return found_attributes;
}
