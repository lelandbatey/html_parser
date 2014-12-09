#include <iostream>
#include <vector>
#include <string>

#include "xml_node.cc"
#include "parse_str.h"

using namespace std;

class XmlTree
{
public:
	XmlTree();
	XmlTree(string& input_document);
	
	void build_xml_tree(string& input_document);
	void print_xml_tree();

	vector<string> find_all_attributes(string attribute_value);

private:
	XmlNode _root;
	void build_tree(ParseStr& document);
	void find_attrs(string attr_val, XmlNode* node, vector<string>* found);
};

