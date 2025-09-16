#pragma once
/**
* author Yme Brugts (s4536622)
 * @file lexer.h
 * @date 2024-11-18
 */

#include <vector>
#include <string>

enum class TokenType {
    Letter,
    Or,
    Star,
    OpenParen,
    CloseParen,
    EndOfInput
};

struct Token {
    TokenType type;
    char value; // For letters
};

std::vector<Token> tokenize(const std::string& input);

