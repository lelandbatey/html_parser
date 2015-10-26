
#include <iostream>
#include <string>

#include "../include/dbg.hpp"
#include "../include/lazy_string.hpp"
#include "../include/html_lexer.hpp"



int main(){
	auto test_str = std::string("<html>   <body><p>This is a test.</p></body></html>");
	auto lex = htmlparser::HtmlLexer(test_str);
	auto tokens = lex.get_tokens();
	PRINT_VECTOR(tokens);
	return 0;
}

