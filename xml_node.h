
#ifndef PARSER_H
#define PARSER_H


#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

using namespace std;


class XmlNode
{
public:
	XmlNode(string type);
	XmlNode(string type, map<string, string> attributes);
	XmlNode(XmlNode* parent, string type, map<string, string> attributes);
	XmlNode(XmlNode* parent, string type, string contents);

	bool has_children();
	bool is_root();
	bool has_text();
	bool has_attribute(string key);

	string get_text();
	XmlNode* get_parent();
	vector<XmlNode*> get_children();
	string get_attribute(string key);
	string get_type();
	int get_depth();

	void add_child(XmlNode* child);

	
private:
	string _type;
	map<string, string> _attributes;
	vector<XmlNode*> _children;
	string _contents;
	XmlNode* _parent;

};


#endif