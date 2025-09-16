#pragma once
/**
* author Yme Brugts (s4536622)
 * @file parser.h
 * @date 2024-11-18
 */

#include "lexer.h"
#include "../models/automaton.h"
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentIndex;
    int stateCounter; // To assign unique state indices

    // Automaton representation as a table
    std::vector<AutomatonState> automaton;

    // Parsing and automaton construction functions
    Fragment parseExpression(bool isTopLevel);
    Fragment parseTerm();
    Fragment parseFactor();
    Fragment parseLetter();
    std::string tokenTypeToString(TokenType type);

    Token currentToken() const;
    void advanceToken();

public:
    Parser(const std::vector<Token>& tokens);
    void parse();
    const std::vector<AutomatonState>& getAutomaton() const;
    void printAutomaton(std::ostream& out) const;
};

