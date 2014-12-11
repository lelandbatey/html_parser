#include <iostream>
#include <vector>
#include <string>
#include <cctype> /* isspace() */
#include <map>

#include "html_parser.h"
#include "lazy_string.h"

#ifndef DEBUG
#define DEBUG 0
#endif

using namespace std;

htmlparser::HtmlParser::HtmlParser(): _root(std::string("root")){}
htmlparser::HtmlParser::HtmlParser(std::string& input_document): _root(std::string("root")){
    this->parse(input_document);
}


// Returns the type of a token representing an opening or closing tag.
std::string htmlparser::get_type(std::string token){
    std::string type("");
    int offset;
    if (is_opening_tag(token)){
        offset = 1;
    } else if (is_closing_tag(token)){
        offset = 2;
    }

    for (unsigned int i = offset; i < token.size(); ++i){
        if (!isspace(token[i]) && token[i] != '>'){
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
bool htmlparser::is_opening_tag(std::string token){
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
bool htmlparser::is_closing_tag(std::string token){
    if (token.length() > 3){
        if (token[0] == '<' && token[1] == '/'){
            return true;
        }
    }
    return false;
}
bool htmlparser::is_self_closing_tag(std::string token){
    if (token.length() > 3){
        if (is_opening_tag(token) && token[token.size()-2] == '/'){
            return true;
        } else if (get_type(token) == "link"){
            return true;
        }
    }
    return false;
}
bool htmlparser::is_comment_tag(std::string token){
    if (str_at_position("<!--", &token, 0)){
        return true;
    }
    return false;
}
bool htmlparser::is_doctype_tag(std::string token){
    if (!is_comment_tag(token) && token[1] == '!'){
        return true;
    }
    return false;
}
bool htmlparser::is_script_tag(std::string token){
    if (str_at_position("<script", &token, 0)){
        return true;
    }
    return false;
}
bool htmlparser::is_text_tag(std::string token){
    if (token.find_first_of("<>") == std::string::npos){
        return true;
    }
    return false;
}

std::string htmlparser::clean_tag(std::string token){
    int trim, start;
    if (is_closing_tag(token)){
        start = 2; trim = 3;
    } else if (is_self_closing_tag(token)){
        start = 1; trim = 3;
    } else if (is_opening_tag(token)){
        start = 1; trim = 2;
    }

    std::string trimmed = token.substr(start, token.size()-trim);
    return trimmed;
}

std::string htmlparser::remove_quotes(std::string quoted_str){
    // Assumes that the string we've been given begins and ends with quotes.
    return quoted_str.substr(1, quoted_str.size()-2);
}


std::vector<std::string> htmlparser::tokenize_opening_tags(std::string opening_tag){
    opening_tag = htmlparser::clean_tag(opening_tag);
    std::vector<std::string> tokens;
    
    bool in_quotes = false, escaped = false;
    std::string buffer("");
    char delim = 0;

    for (unsigned int i = 0; i < opening_tag.size(); ++i){
        char cc = opening_tag[i];
        if (!in_quotes && (cc == ' ' || cc == '>' || cc == '/')){
            // Important that these two if statements be separate. If buffer
            // has no size, then we do nothing, which is intended behaviour.
            if (buffer.size()){
                tokens.push_back(buffer);
                buffer = std::string("");
            }
        } else if (!in_quotes && cc == '='){
            // Tokenizes equals symbol as it's own token
            if (buffer.size()){
                tokens.push_back(buffer);
                tokens.push_back(std::string("="));
                buffer = std::string("");
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

std::map<std::string, std::string> htmlparser::get_attributes(std::string token){
    std::map<std::string, std::string> attributes;

    std::vector<std::string> attr_tokens = tokenize_opening_tags(token);
    std::string temp_key(""), temp_value("");
    

    if (attr_tokens.size() < 2){
        return attributes;
    }

    for (unsigned int i = 1; i < attr_tokens.size(); ++i){
        std::string temp_key = attr_tokens[i];
        std::string temp_value = "";

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


void htmlparser::HtmlParser::parse(std::string& input_document){
    std::map<std::string, std::string> attributes;
    _lexer = htmlparser::HtmlLexer(input_document);
    XmlNode* root = &_root;

    XmlNode* current_node = root;
    std::string type("");

    while (!_lexer.eof()){
        std::string token = _lexer.get_next_token();

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
                    std::cerr << tn->get_type() << std::endl;
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
void htmlparser::HtmlParser::recursive_tree_printer(XmlNode* node){
    
    for (int i = 0; i < node->get_depth(); ++i){
        std::cout << "    ";
    }
    if (node->has_text()){
        std::cout << node->get_text() << std::endl;
    } else {
        std::cout << "<" << node->get_type() << ">" << std::endl;
    }

    if (node->has_children()){
        std::vector<XmlNode*> children = node->get_children();
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

void htmlparser::HtmlParser::print(){
    if (_root.has_children()){
        std::vector<XmlNode*> children = _root.get_children();
        for (unsigned int i = 0; i < children.size(); ++i){
            recursive_tree_printer(children[i]);
        }
    }
}

void htmlparser::HtmlParser::find_attrs(std::string attr_val, XmlNode* node, std::vector<std::string>* found){

    if (node->has_attribute(attr_val)){
        found->push_back(node->get_attribute(attr_val));
    }

    if (node->has_children()){
        std::vector<XmlNode*> children = node->get_children();
        for (unsigned int i = 0; i < children.size(); ++i){
            find_attrs(attr_val, children[i], found);
        }
    }
}

// Trampolines into the "find_attrs" function
std::vector<std::string> htmlparser::HtmlParser::find_all_attributes(std::string attribute_name){
    std::vector<std::string> found_attributes;
    this->find_attrs(attribute_name, &_root, &found_attributes);
    return found_attributes;
}
