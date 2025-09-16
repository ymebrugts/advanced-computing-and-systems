/**
 * @author Yme Brugts (s4536622)
 * @file tokenizer.cpp
 * @date 2024-10-09
 */
#include "tokenizer.h"

/**
 * @function tokenize
 * @abstract Tokenizes a given string into individual tokens based on whitespace.
 * @param str The input string (`const std::string&`) to be tokenized.
 * @return A queue (`std::queue<std::string>`) containing the individual tokens extracted from the input string.
 * @pre The input string `str` must be a valid, well-formed string without embedded null characters.
 * @post - Returns a queue containing all tokens from the input string.
 *       - The input string remains unmodified.
 **/
std::queue<std::string> tokenize(const std::string& str) {
    std::istringstream iss(str);
    std::queue<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push(token);
    }
    return tokens;
}