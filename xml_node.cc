#include <algorithm>
#include <iostream>
#include <string>

#include "xml_node.h"
#include "parse_str.h"

#pragma once

using namespace std;



bool XmlNode::has_children(){
	if (!_children.empty()){
		return true;
	}
	return false;
}

bool XmlNode::is_root(){
	if (!_parent){
		return true;
	}
	return false;
}

bool XmlNode::has_text(){
	if (_contents.length()){
		return true;
	}
	return false;
}

bool XmlNode::has_attribute(string key){
	key = strLower(key);
	if (_attributes.count(key) > 0){
		return true;
	}
	return false;
}

string XmlNode::get_text(){ return _contents; }
XmlNode* XmlNode::get_parent(){ return _parent; }
vector<XmlNode*> XmlNode::get_children(){ return _children; }
string XmlNode::get_type(){ return _type; }
string XmlNode::get_attribute(string key){
	key = strLower(key);
	if (_attributes.count(key) > 0){
		return _attributes[key];
	} else {
		return string("");
	}
}
int XmlNode::get_depth(){
	if (!is_root()){
		return _parent->get_depth()+1;
	} else {
		return -1;
	}
}

void XmlNode::add_child(XmlNode* child){
	_children.push_back(child);
}

XmlNode::XmlNode(string type):_type(strLower(type)), _parent(0){}

XmlNode::XmlNode(string type, map<string, string> attributes):
	_type(strLower(type)), _attributes(attributes), _parent(0){}

XmlNode::XmlNode(XmlNode* parent, string type, string contents):
	_type(strLower(type)), _contents(contents), _parent(parent){

	_parent->add_child(this);
}

XmlNode::XmlNode(XmlNode* parent, string type, map<string, string> attributes):
	_type(strLower(type)), _attributes(attributes), _parent(parent){
	_parent->add_child(this);
}


