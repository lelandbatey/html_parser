#include <iostream>
#include <vector>
#include <string>
#include <cctype> /* isspace() */
#include <map>

#include "../include/html_parser.hpp"
#include "../include/lazy_string.hpp"

#ifndef DEBUG
#define DEBUG 0
#endif

using namespace std;

htmlparser::HtmlParser::HtmlParser(): _root(std::string("root")), _is_valid_html(true){}
htmlparser::HtmlParser::HtmlParser(std::string& input_document): _root(std::string("root")), _is_valid_html(true){
	this->parse(input_document);
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
				// Handles case where a void-element has a closing tag when
				// it's not supposed to, such as <link></link>
				if (is_void_element(token) &&
					get_type(_lexer.peak_next_token()) == type){
					token = _lexer.get_next_token();
					_is_valid_html = false;
				}
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
				_is_valid_html = false;
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


bool htmlparser::HtmlParser::is_valid_html(){
	return _is_valid_html;
}
