#ifndef HTML_LEXER_H
#define HTML_LEXER_H

#include <iostream>
#include <vector>
#include <string>


namespace htmlparser {

	class HtmlLexer
	{
	public:
		HtmlLexer();
		HtmlLexer(std::string& input_document);

		void tokenize(std::string& input_document);
		std::string get_next_token();
		std::string peak_next_token();
		std::vector<std::string> get_tokens();
		bool eof();
		void print_tokens();

	private:
		std::vector<std::string> _tokens;
		unsigned int _current_token_index;
		bool _eof;
	};

	bool has_only_whitespace(std::string& str);
}




#endif