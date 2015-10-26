#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <iostream>
#include <vector>
#include <string>

#include "../include/xml_node.hpp"
#include "../include/html_lexer.hpp"
#include "../include/html_util.hpp"

using namespace std;

/**
 * @brief
 * @details
 *
 * @param
 * @return
 */


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
}

#endif
