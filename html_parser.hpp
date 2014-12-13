#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <iostream>
#include <vector>
#include <string>

#include "xml_node.cpp"
#include "html_lexer.cpp"

using namespace std;

namespace htmlparser {
	
	class HtmlParser
	{
	public:
		HtmlParser();
		HtmlParser(std::string& input_document);
		
		void parse(std::string& input_document);
		void print();
		void recursive_tree_printer(XmlNode* node);

		std::vector<std::string> find_all_attributes(std::string attribute_name);

		bool is_valid_html();

	private:
		void find_attrs(std::string attr_val, XmlNode* node, std::vector<std::string>* found);
		HtmlLexer _lexer;
		XmlNode _root;
		bool _is_valid_html;
	};

	std::string get_type(std::string token);

	bool is_opening_tag(std::string token);
	bool is_closing_tag(std::string token);
	bool is_self_closing_tag(std::string token);
	bool is_void_element(std::string token);
	bool is_comment_tag(std::string token);
	bool is_doctype_tag(std::string token);
	bool is_script_tag(std::string token);
	bool is_text_tag(std::string token);

	std::string clean_tag(std::string token);
	std::string remove_quotes(std::string quoted_str);
	std::vector<std::string> tokenize_opening_tags(std::string opening_tag);
	std::map<std::string, std::string> get_attributes(std::string token);

}



#endif