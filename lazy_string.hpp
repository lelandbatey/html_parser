#ifndef HTMLPARSER_LAZY_STR_H
#define HTMLPARSER_LAZY_STR_H
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>


using namespace std;

// Contains all my lazy functions for string manipulation. This *cannot* be a
// good or wise thing to do, but I don't know what I'm supposed to do with
// this...

namespace htmlparser {

	// trim from start
	static inline std::string &ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
			return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
			return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
			return ltrim(rtrim(s));
	}


	std::string dubToStr(double dub) {
		std::ostringstream temp;

		temp << dub;
		
		std::string result(temp.str());
		return result; 
	}

	int upper(int c){
		return toupper((unsigned char)c);
	}
	int lower(int c){
		return tolower((unsigned char)c);
	}
	std::string strLower(std::string str){
		std::transform(str.begin(), str.end(), str.begin(), lower); // Make it lowercase
		return str;
	}
	std::string strUpper(std::string str) {
		std::transform(str.begin(), str.end(), str.begin(), upper); // Make it uppercase

		return str;
	}


	// Checks if the given index in a string contains the given key
	bool str_at_position(string key, string* scan_str, unsigned int index){
		// Keeps us from trying to read past the end of a string
		unsigned int len = scan_str->length() - index;
		if (len > key.length()){
			len = key.length();
		}

		for (unsigned int i = 0; i < len; ++i){
			if ((*scan_str)[i+index] != key[i]){
				return false;
			}
		}
		return true;
	}
}

#endif
