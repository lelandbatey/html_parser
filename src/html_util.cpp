
#include <iostream>
#include <vector>
#include <string>
//#include <cctype> /* isspace() */
#include <map>

#include "../include/html_util.hpp"
#include "../include/lazy_string.hpp"


// Returns the type of a token representing an opening or closing tag.
std::string htmlparser::get_type(std::string token){
	std::string type("");
	int offset = 0;
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
		} else if (is_void_element(token)){
			return true;
		}
	}
	return false;
}
bool htmlparser::is_void_element(std::string token){
	std::string type = get_type(token);
	// List of void elements taken from the HTML5 specification:
	//     http://www.w3.org/TR/html5/syntax.html#void-elements
	std::vector<std::string> void_elements = {"area", "base", "br", "col",
		"embed", "hr", "img", "input", "keygen", "link", "meta", "param", "source", "track", "wbr"};

	for (unsigned int i = 0; i < void_elements.size(); ++i){
		if (type == void_elements[i]){
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
	int trim = 0, start = 0;
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

// Returns a map of the attributes of a given html element
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

