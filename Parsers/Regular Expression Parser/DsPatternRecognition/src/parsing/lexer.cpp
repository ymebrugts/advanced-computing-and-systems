/**
* author Yme Brugts (s4536622)
 * @file lexer.cpp
 * @date 2024-11-18
 */


#include "lexer.h"
#include <stdexcept>
#include <cctype>

/**
 * Processes the input string and converts it into a sequence of tokens.
 *
 * This function reads through the input string and categorizes each character
 * into predefined token types. Supported token types include letters (lowercase
 * 'a' to 'z'), as well as the characters '|', '*', '(', and ')'. Whitespace is
 * ignored, and any invalid character will cause a runtime exception to be thrown.
 *
 * @param input The string to be tokenized.
 * @return A vector of tokens representing the categorized characters from the input.
 * @throws std::runtime_error If an invalid character is encountered in the input.
 */
std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t index = 0;

    while (index < input.length()) {
        char ch = input[index];
        if (std::isspace(ch)) {
            ++index;            
        }
        else if (ch >= 'a' && ch <= 'z') {
            tokens.push_back({ TokenType::Letter, ch });
            ++index;
        }
        else if (ch == '|') {
            tokens.push_back({ TokenType::Or, 0 });
            ++index;
        }
        else if (ch == '*') {
            tokens.push_back({ TokenType::Star, 0 });
            ++index;
        }
        else if (ch == '(') {
            tokens.push_back({ TokenType::OpenParen, 0 });
            ++index;
        }
        else if (ch == ')') {
            tokens.push_back({ TokenType::CloseParen, 0 });
            ++index;
        }
        else {
            throw std::runtime_error(std::string("Invalid character: ") + ch);
        }
    }

    tokens.push_back({ TokenType::EndOfInput, 0 });
    return tokens;
}
