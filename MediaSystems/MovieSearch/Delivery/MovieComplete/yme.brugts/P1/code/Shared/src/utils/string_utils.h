#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file string_utils.h
 * @date 2025-09-16
 */

#include <string>
#include <vector>

namespace shared::utils {

    /**
     * @brief Remove leading whitespace from a string
     * @param s Input string
     * @return String with leading whitespace removed
     */
    std::string ltrim(std::string s);

    /**
     * @brief Remove trailing whitespace from a string
     * @param s Input string
     * @return String with trailing whitespace removed
     */
    std::string rtrim(std::string s);

    /**
     * @brief Remove leading and trailing whitespace from a string
     * @param s Input string
     * @return String with leading and trailing whitespace removed
     */
    std::string trim(std::string s);

    /**
     * @brief Replace all occurrences of a substring in a string
     * @param s String to modify
     * @param from Substring to replace
     * @param to Replacement substring
     */
    void replaceAll(std::string& s, const std::string& from, const std::string& to);

    /**
	 * @brief Split a string by a delimiter into tokens
	 * @param s Input string
	 * @param delimiter Delimiter string (default "::")
	 * @return Vector of tokens
	 */
    std::vector<std::string> split(const std::string& s, const std::string& delimiter = "::");

	/**
	 * @brief Check if a word is present in text, ignoring case
	 * @param text Input text
	 * @param word Word to search for
	 * @return True if the word is found, false otherwise
	 */
    bool case_insensitive_contains_word(const std::string& text, const std::string& word);

	/**
	 * @brief Join a vector of strings into a single string
	 * @param vec Vector of strings
	 * @param delimiter Delimiter to place between strings (default "|")
	 * @return Concatenated string with delimiters
	 */
    std::string join(const std::vector<std::string>& vec, const std::string& delimiter = "|");

}
