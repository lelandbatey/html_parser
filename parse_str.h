#ifndef PARSE_STR_H
#define PARSE_STR_H
#include <string>
#include <iostream>
#include <sstream>

#ifndef _DEBUG_
#define _DEBUG_ 0
#endif

using namespace std;


// A wrapper around a string to allow for easy retrieval of single characters.
class ParseStr
{
public:
	ParseStr(string input):_str(input), _pos(0), _eof(false){};

	char get(){
		char cc;

		if (_pos < _str.size()){
			cc = _str[_pos];
			++_pos;
		} else {
			cc = '\0';
			_eof = true;
		}
		return cc;
	}

	bool eof(){
		if (_pos == _str.size()){
			_eof = true;
		}
		return _eof;
	}

	void dump(){
		if (_DEBUG_){
			for (unsigned int i = 0; i < _pos; ++i){
				std::cout << _str[i];
			}
			std::cout << std::endl;
		}
	}

	void shift_back(unsigned int back){
		_pos -= back;
	}


private:
	string _str;
	unsigned int _pos;
	bool _eof;
	
};

std::string dubToStr(double dub) {
    std::ostringstream temp;

    temp << dub;
    
    std::string result(temp.str());
    return result; 
}

int upper(int c){
  return std::toupper((unsigned char)c);
}
int lower(int c){
  return std::tolower((unsigned char)c);
}
std::string strLower(std::string str){
    std::transform(str.begin(), str.end(), str.begin(), lower); // Make it lowercase
    return str;
}
std::string strUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), upper); // Make it uppercase

    return str;
}





#endif