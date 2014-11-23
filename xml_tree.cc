#include <iostream>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <memory>
#include <locale>
#include <map>

#include "xml_tree.h"
#include "parse_str.h"


XmlTree::XmlTree(): _root(string("root")){}
XmlTree::XmlTree(string& input_document): _root(string("root")){
	build_xml_tree(input_document);
}



void XmlTree::build_xml_tree(string& input_document){
	ParseStr doc(input_document);
	// istringstream doc(input_document);
	build_tree(doc);
}

// Trampoline into the recursive search below
vector<string> XmlTree::find_all_attributes(string attribute_value){
	vector<string> found_attributes;

	find_attrs(attribute_value, &_root, &found_attributes);
	return found_attributes;
}

void XmlTree::find_attrs(string attr_val, XmlNode* node, vector<string>* found){

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

void xml_tree_printer(XmlNode* node){
	for (int i = 0; i < node->get_depth(); ++i){
		std::cout << "    ";
	}
	if (node->has_text()){
		std::cout << "'" << node->get_text() << "'" << std::endl;
	} else {
		std::cout << "<" << node->get_type() << ">" << std::endl;
	}

	if (node->has_children()){
		vector<XmlNode*> children = node->get_children();
		for (unsigned int i = 0; i < children.size(); ++i){
			xml_tree_printer(children[i]);
		}
	}

	if (!node->has_text()){
		for (int i = 0; i < node->get_depth(); ++i){
			std::cout << "    ";
		}
		std::cout << "</" << node->get_type() << ">" << std::endl;
		
	}

}

void XmlTree::print_xml_tree(){
	xml_tree_printer(&_root);
}

// Macro for conveniently clearing all variables.
#ifndef _CLEAR_
#define _CLEAR_ state = ""; \
type = ""; \
tmp_value = ""; \
tmp_key = ""; \
tmp_type = ""; \
attributes.clear(); 
#endif

void XmlTree::build_tree(ParseStr& document){
	// istringstream& document = *input_document; 
	XmlNode* root = &_root;

	XmlNode* parent = root;
	map<string, string> attributes;
	string contents;
	string type;
	string tmp_key, tmp_value;
	string tmp_type;

	string buffer;

	// State will be our flag for which part of the parsing process we are
	// currently doing. Possible states are:
	//
	// 1. Opening tag
	//     - Type
	//     - Attributes
	//         - Key
	//         - Value
	// 2. Closing tag
	// 3. Doctype tag
	// 4. Text value
	// 5. Comment
	// 6. Unknown
	string state("");

	while (!document.eof()){
		// Stands for "current character"
		char cc;
		cc = document.get();

		if (document.eof()){
			continue;
		}

		if (state.empty()){
			if (cc == '<'){
				state = "opening";
			} else {
				state = "text";
				contents.push_back(cc);
			}
		} else if (state == "opening" || state == "type"){
			if (!isspace(cc) && cc != '>' && cc != '/'){
				if (cc == '!'){
					state = "doctype";
				} else {				
					state = "type";
					type.push_back(cc);
				}
			} else if (type.size() && isspace(cc)){
				state = "key";
			} else if (!type.size() && cc == '/'){
				state = "closing";
			} else if (cc == '>'){
				parent = new XmlNode(parent, type, attributes);
				_CLEAR_;
			}
		} else if (state == "doctype"){
			buffer.push_back(cc);
			if (buffer == "--"){
				state = "comment";
				buffer = "";
			}
			if (cc == '>'){
				state = "";
				buffer = "";
			}
		} else if (state == "comment") {
			if (cc == '-'){
				buffer.push_back(cc);
			} else if (cc == '>' && buffer == "--"){
				state = "";
				buffer = "";
			} else {
				if (isalnum(cc)){
					buffer = "";
				}
			}
		} else if (state == "key"){
			if (!isspace(cc) && cc != '=' && cc != '>'){
				// If there was a key but no value, and we're now looking at a second key.
				if (tmp_key.size() && attributes.count(tmp_key)){
					tmp_key = "";
				}
				tmp_key.push_back(cc);
			} else if (tmp_key.size() && cc == '='){
				state = "value";
			} else if (isspace(cc) && tmp_key.size()){
				attributes[strLower(tmp_key)] = "";
			} else if (cc == '>'){
				parent = new XmlNode(parent, type, attributes);
				_CLEAR_;
			}

			if (type.size() && cc == '/'){
				state = "closing";
				// Do this so if we immediately close this tag
				buffer = "/";
			}
		} else if (state == "value"){

			// Macro for smaller saves
			#define _SAVE_VAL_ \
			attributes[strLower(tmp_key)] = tmp_value;\
			tmp_value = ""; \
			tmp_key = ""; \

			// We get the value of things in quotes
			char delim = 0;
			if (cc == '"' || cc == '\''){
				delim = cc;
			}
			if (delim){
				bool escaped = false;
				cc = document.get();

				while (escaped || cc != delim){
					if (!escaped && cc == '\\'){
						escaped = true;
					} else {
						tmp_value.push_back(cc);
						escaped = false;
					}
					cc = document.get();
				}
				_SAVE_VAL_;
				state = "key";
			} else{
				// Stuff isn't always in quotes though, so we have to go till we
				// hit a space or a "/" or a ">"
				while (!(isspace(cc) || cc == '/' || cc == '>')){
					tmp_value.push_back(cc);
					cc = document.get();
				}

				if (cc == ' '){
					_SAVE_VAL_;
					state = "key";
				} else if (cc == '/'){
					// For a self-closing tag
					_SAVE_VAL_;

					buffer = "/";
					state = "closing";
				} else if (cc == '>'){
					// For a self-closing tag
					_SAVE_VAL_;

					// So that the correct case is actually handled.
					document.shift_back(1);
					state = "key";
				}
			}
		} else if (state == "text"){
			if (cc != '<'){
				contents.push_back(cc);
			} else {
				// Prevents just whitespace from being made into a text node.
				for (unsigned int i = 0; i < contents.length(); ++i){
					if (!isspace(contents.c_str()[i])){
						new XmlNode(parent, string("text"), contents);
						break;
					}
				}

				contents = "";
				state = "opening";
			}
		} else if (state == "closing"){
			if (cc == '>'){

				// Case which handles self closing tags like <input />
				if (buffer == "/"){
					buffer = "";
					new XmlNode(parent, type, attributes);
					_CLEAR_;
				} else {
					// std::cout << "'" << buffer << "'" << std::endl;		
					XmlNode* tn = parent;
					while (tn->get_type() != tmp_type){
						if (tn->is_root()){
							// document.dump();
							// print_xml_tree();

							// xml_tree_printer(parent);
							// std::cout << "'" << parent->get_attribute("value") << "'" << std::endl;
							// Force a segfault out of rage
							// std::cout << "THIS XML DOCUMENT IS STUPID AND I HATE IT. WE'RE DONE!" << std::endl;
							std::cout << "XML Document is malformed." << std::endl;
							return;
							// tn = 0;
							// tn->is_root();
						}
						tn = tn->get_parent();
					}

					parent = tn->get_parent();
					_CLEAR_;
				}
			} else {

				tmp_type.push_back(cc);
			}
		}
	}
}


