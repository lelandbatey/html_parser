#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

#include "html_parser.cpp"

using namespace std;



int main(int argc, char const *argv[]){
    
    ifstream t("index.html");
    string input((istreambuf_iterator<char>(t)),
                  istreambuf_iterator<char>());

    // HtmlParser parser(input);
    htmlparser::HtmlLexer lexer(input);

    std::cout << htmlparser::get_type("</a\n>") << std::endl;


    return 0;
}