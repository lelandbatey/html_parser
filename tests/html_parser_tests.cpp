
#include <iostream>
#include <string>

#include "../include/dbg.hpp"
#include "../include/minunit.hpp"
#include "../include/lazy_string.hpp"
#include "../include/html_lexer.hpp"
#include "../include/html_parser.hpp"


const char* test_clean_tags(){
	auto tag = std::string("<opening>");
	auto tag0 = std::string("<selfclosing/>");
	auto tag1 = std::string("</closing>");
	auto tag2 = std::string("</ closingspaced >");

	auto ret = htmlparser::clean_tag(tag);
	auto ret0 = htmlparser::clean_tag(tag0);
	auto ret1 = htmlparser::clean_tag(tag1);
	auto ret2 = htmlparser::clean_tag(tag2);

	//std::cout << "ret: " << ret << std::endl;
	//std::cout << "ret0: " << ret0 << std::endl;
	//std::cout << "ret1: " << ret1 << std::endl;
	//std::cout << "ret2: " << ret2 << std::endl;
	mu_assert(ret == "opening", "'<opening>' tag not properly cleaned.");
	mu_assert(ret0 == "selfclosing", "'<selfclosing/>' tag not properly cleaned.");
	mu_assert(ret1 == "closing", "'</closing>' tag not properly cleaned.");
	mu_assert(ret2 == " closingspaced ", "'</ closingspaced >' tag not properly cleaned.");
	return NULL;
}

//char* test_


const char* all_tests(){
	mu_suite_start();

	mu_run_test(test_clean_tags);


	return NULL;
}



RUN_TESTS(all_tests);

//int main(){
	//auto test_str = std::string("<html>   <body><p>This is a test.</p></body></html>");
	//auto lex = htmlparser::HtmlLexer(test_str);
	//auto tokens = lex.get_tokens();
	//PRINT_VECTOR(tokens);

	////auto t =  
	//return 0;
//}
