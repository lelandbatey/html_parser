#ifndef LAZY_STR_H
#define LAZY_STR_H
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// Contains all my lazy functions for string manipulation. This *cannot* be a
// good or wise thing to do, but I don't know what I'm supposed to do with
// this...

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
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