#include <iostream>
#include <string>

#include "xml_node.h"
#include "lazy_string.h"

#pragma once


bool htmlparser::XmlNode::has_children(){
	if (!_children.empty()){
		return true;
	}
	return false;
}

bool htmlparser::XmlNode::is_root(){
	if (!_parent){
		return true;
	}
	return false;
}

bool htmlparser::XmlNode::has_text(){
	if (_contents.length()){
		return true;
	}
	return false;
}

bool htmlparser::XmlNode::has_attribute(std::string key){
	key = htmlparser::strLower(key);
	if (_attributes.count(key) > 0){
		return true;
	}
	return false;
}

std::string htmlparser::XmlNode::get_text(){ return _contents; }
htmlparser::XmlNode* htmlparser::XmlNode::get_parent(){ return _parent; }
std::vector<htmlparser::XmlNode*> htmlparser::XmlNode::get_children(){ return _children; }
std::string htmlparser::XmlNode::get_type(){ return _type; }
std::string htmlparser::XmlNode::get_attribute(std::string key){
	key = htmlparser::strLower(key);
	if (_attributes.count(key) > 0){
		return _attributes[key];
	} else {
		return std::string("");
	}
}
int htmlparser::XmlNode::get_depth(){
	if (!is_root()){
		return _parent->get_depth()+1;
	} else {
		return -1;
	}
}

void htmlparser::XmlNode::add_child(XmlNode* child){
	_children.push_back(child);
}

htmlparser::XmlNode::XmlNode(std::string type):_type(htmlparser::strLower(type)), _parent(0){}

htmlparser::XmlNode::XmlNode(std::string type, std::map<std::string, std::string> attributes):
	_type(htmlparser::strLower(type)), _attributes(attributes), _parent(0){}

htmlparser::XmlNode::XmlNode(XmlNode* parent, std::string type, std::string contents):
	_type(htmlparser::strLower(type)), _contents(contents), _parent(parent){

	_parent->add_child(this);
}

htmlparser::XmlNode::XmlNode(XmlNode* parent, std::string type, std::map<std::string, std::string> attributes):
	_type(htmlparser::strLower(type)), _attributes(attributes), _parent(parent){
	_parent->add_child(this);
}


