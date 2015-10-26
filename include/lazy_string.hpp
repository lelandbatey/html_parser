#ifndef HTMLPARSER_LAZY_STR_H
#define HTMLPARSER_LAZY_STR_H
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// Contains all my lazy functions for string manipulation. This *cannot* be a
// good or wise thing to do, but I don't know what I'm supposed to do with
// this...

namespace htmlparser {

	// trim from start
	inline std::string &ltrim(std::string &s); 

	// trim from end
	inline std::string &rtrim(std::string &s); 

	// trim from both ends
	inline std::string &trim(std::string &s); 


	std::string dubToStr(double dub); 

	int upper(int c);
	int lower(int c);
	std::string strLower(std::string str);
	std::string strUpper(std::string str); 


	// Checks if the given index in a string contains the given key
	bool str_at_position(string key, string* scan_str, unsigned int index);
}


#endif
