
#ifndef PARSER_H
#define PARSER_H


#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

using namespace std;

namespace htmlparser {

	class XmlNode
	{
	public:
		XmlNode(std::string type);
		XmlNode(std::string type, std::map<std::string, std::string> attributes);
		XmlNode(XmlNode* parent, std::string type, std::map<std::string, std::string> attributes);
		XmlNode(XmlNode* parent, std::string type, std::string contents);

		bool has_children();
		bool is_root();
		bool has_text();
		bool has_attribute(std::string key);

		std::string get_text();
		XmlNode* get_parent();
		std::vector<XmlNode*> get_children();
		std::string get_attribute(std::string key);
		std::string get_type();
		int get_depth();

		void add_child(XmlNode* child);

		
	private:
		std::string _type;
		std::map<std::string, std::string> _attributes;
		std::vector<XmlNode*> _children;
		std::string _contents;
		XmlNode* _parent;

	};
}



#endif