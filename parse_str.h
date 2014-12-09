#ifndef PARSE_STR_H
#define PARSE_STR_H
#include <string>
#include <iostream>
#include <sstream>

#ifndef _DEBUG_
#define _DEBUG_ 0
#endif

using namespace std;

// Contains all my lazy functions for string manipulation. This *cannot* be a
// good or wise thing to do, but I don't know what I'm supposed to do with
// this...

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