#ifndef HTML_UTIL_H
#define HTML_UTIL_H

#include <vector>
#include <string>

namespace htmlparser {
	/**
	 * @brief Returns the type of a tag
	 * @details Given a string representing a tag, returns the type of that
	 * tag. For example, the tag `<html>` has a type of `html`. The tag `<br/>
	 * has a type of `br`.
	 *
	 * @param token String representing a tag.
	 * @return Returns a string representing the type of the tag given as input.
	 */
	std::string get_type(std::string token);

	bool is_opening_tag(std::string token);
	bool is_closing_tag(std::string token);
	bool is_self_closing_tag(std::string token);
	bool is_void_element(std::string token);
	bool is_comment_tag(std::string token);
	bool is_doctype_tag(std::string token);
	bool is_script_tag(std::string token);
	bool is_text_tag(std::string token);

	/**
	 * @brief Removes the braces from a tag
	 * @details Removes the opening brace and closing brace from a tag, leaving
	 * only the type and attributes. Correctly handles opening tags, closing
	 * tags, and self closing tags.
	 *
	 * @param token String representing a tag.
	 * @return Returns a tag with it's opening and closing braces, and associated backslash, removed.
	 */
	std::string clean_tag(std::string token);
	std::string remove_quotes(std::string quoted_str);
	/**
	 * @brief Tokenizes an opening tag.
	 * @details Given an opening tag, tokenize it into it's component parts.
	 *
	 * @param opening_tag String representing an opening tag.
	 * @return Returns a vector of strings, each representing a token.
	 */
	std::vector<std::string> tokenize_opening_tags(std::string opening_tag);
	std::map<std::string, std::string> get_attributes(std::string token);

}


#endif
