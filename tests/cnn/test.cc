#include <iostream>
#include <fstream>
#include <streambuf>

#include "../../html_parser.cc"

// Tests to see if the index.html file in the current directory contains valid
// html or not.
int main(int argc, char const *argv[]){
    ifstream t("index.html");
    string input((istreambuf_iterator<char>(t)),
                  istreambuf_iterator<char>());

    htmlparser::HtmlParser parser(input);

    if (parser.is_valid_html()){
        std::cout << "valid" << std::endl;
        return 0;
    }
    return 1;
}

