/**
* author Yme Brugts (s4536622)
 * @file parser.cpp
 * @date 2024-11-18
 */

#include "parser.h"
#include <stdexcept>
#include <sstream>
#include <string>

/**
 * Constructs a Parser object with a given vector of tokens, initializing
 * the internal index and state counter to zero.
 *
 * @param tokens A const reference to a vector of Token objects that will
 *               be parsed by this Parser instance.
 *
 * @return An instance of the Parser class with the provided tokens, ready
 *         for parsing operations.
 */
Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentIndex(0), stateCounter(0) {}


/**
 * Converts a TokenType enum to its corresponding string representation.
 *
 * @param type The TokenType value to be converted.
 *
 * @return A string corresponding to the TokenType. Returns "Letter" for
 *         TokenType::Letter, "Or" for TokenType::Or, "Star" for
 *         TokenType::Star, "OpenParen" for TokenType::OpenParen,
 *         "CloseParen" for TokenType::CloseParen, and "EndOfInput" for
 *         TokenType::EndOfInput. If the TokenType is not recognized,
 *         returns "Unknown".
 */
std::string Parser::tokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::Letter: return "Letter";
    case TokenType::Or: return "Or";
    case TokenType::Star: return "Star";
    case TokenType::OpenParen: return "OpenParen";
    case TokenType::CloseParen: return "CloseParen";
    case TokenType::EndOfInput: return "EndOfInput";
    default: return "Unknown";
    }
}

/**
 * Retrieves the current token from the token list.
 *
 * @return The current Token object if the index is within bounds;
 *         otherwise, a Token with type TokenType::EndOfInput.
 */
Token Parser::currentToken() const {
    return currentIndex < tokens.size() ? tokens[currentIndex] : Token{ TokenType::EndOfInput, 0 };
}

/**
 * Advances the internal index to the next token in the list, if possible.
 * If the index is already at the end, it remains unchanged.
 */
void Parser::advanceToken() {
    if (currentIndex < tokens.size()) {
        ++currentIndex;
    }
}

/**
 * Entry point for parsing
 * Parses the input tokens and constructs the corresponding automaton.
 *
 * @throws std::runtime_error If unexpected tokens are encountered during parsing
 *         or if the input is not fully consumed.
 */
void Parser::parse() {
    automaton.clear();

    // Reserve state 0 as the start state
    stateCounter = 1; 

    Fragment frag = parseExpression(true);

    // Ensure the fragment's start state is 0
    // NOTE: To avoid complexity for a|b case (an OR token at the start) an extra startstate is always added.
    //       A solution was created but this added more complexity to the code due to a few edge cases for little gain.
    if (frag.startState != 0) {
        // Add epsilon transition from state 0 to frag.startState
        automaton.insert(automaton.begin(), { 0, '-', frag.startState, -1 });
        frag.startState = 0;
    }

    // Ensure the entire input has been consumed
    if (currentToken().type != TokenType::EndOfInput) {
        throw std::runtime_error("Unexpected token after the end of the expression at position " +
            std::to_string(currentIndex));
    }

    // Mark the accept state
    automaton.push_back({ frag.acceptState, '-', -1, -1 });
}

/**
 * <expr> ::= <term> { '|' <term> }*
 * Parses an expression (<expr>) from the token stream. Expressions may
 * include terms combined with the '|' operator.
 *
 * @param isTopLevel A boolean indicating whether this is the top-level
 *                   expression. Top-level expressions always start at state 0.
 *
 * @return A Fragment representing the parsed expression's start and accept states.
 *
 * @throws std::runtime_error If the expression contains unexpected tokens
 *         or is not well-formed.
 */
Fragment Parser::parseExpression(bool isTopLevel) {
    Fragment leftFrag = parseTerm();

    while (currentToken().type == TokenType::Or) {
        advanceToken(); // Consume '|'
        Fragment rightFrag = parseTerm();

        int startState = isTopLevel ? 0 : stateCounter++;
        int acceptState = stateCounter++;

        // Epsilon transitions from startState to both left and right fragments
        automaton.push_back({ startState, '-', leftFrag.startState, rightFrag.startState });

        // Epsilon transitions from left and right accept states to acceptState
        automaton.push_back({ leftFrag.acceptState, '-', acceptState, -1 });
        automaton.push_back({ rightFrag.acceptState, '-', acceptState, -1 });

        // Update leftFrag to the new combined fragment
        leftFrag = { startState, acceptState };
    }

    return leftFrag;
}

/**
 * <term> ::= <fact> { <fact> }*
 * Parses a term (<term>) from the token stream. Terms are sequences of
 * factors concatenated together.
 *
 * @return A Fragment representing the parsed term's start and accept states.
 *
 * @throws std::runtime_error If the term contains unexpected tokens
 *         or is not well-formed.
 */
Fragment Parser::parseTerm() {
    Fragment leftFrag = parseFactor();

    while (currentToken().type == TokenType::Letter || currentToken().type == TokenType::OpenParen) {
        Fragment rightFrag = parseFactor();

        // Connect the accept state of the left fragment to the start state of the right fragment
        automaton.push_back({ leftFrag.acceptState, '-', rightFrag.startState, -1 });

        // Update the left fragment's accept state
        leftFrag.acceptState = rightFrag.acceptState;
    }

    return leftFrag;
}

/**
 * <fact> ::= <lett> [ * ] | ( <expr> ) [ * ]
 * Parses a factor (<fact>) from the token stream. Factors can be a letter
 * or a parenthesized expression, optionally followed by a Kleene star ('*').
 *
 * @return A Fragment representing the parsed factor's start and accept states.
 *
 * @throws std::runtime_error If the factor is not well-formed or if it
 *         contains unexpected tokens.
 */
Fragment Parser::parseFactor() {
    Fragment baseFrag;

    if (currentToken().type == TokenType::Letter) {
        baseFrag = parseLetter();
    }
    else if (currentToken().type == TokenType::OpenParen) {
        advanceToken(); // Consume '('
        baseFrag = parseExpression(false);

        if (currentToken().type != TokenType::CloseParen) {
            throw std::runtime_error("Expected ')' but found token of type " +
                tokenTypeToString(currentToken().type) +
                " at position " + std::to_string(currentIndex));
        }
        advanceToken(); // Consume ')'
    }
    else {
        throw std::runtime_error("Expected a letter or '(' but found token of type " +
            tokenTypeToString(currentToken().type) +
            " at position " + std::to_string(currentIndex));
    }

    // Handle optional Kleene star
    if (currentToken().type == TokenType::Star) {
        advanceToken(); // Consume '*'

        int startState = stateCounter++;
        int acceptState = stateCounter++;

        // Epsilon transitions from new start state to base fragment and accept state
        automaton.push_back({ startState, '-', baseFrag.startState, acceptState });

        // Epsilon transitions from base fragment accept state back to base start state and to accept state
        automaton.push_back({ baseFrag.acceptState, '-', baseFrag.startState, acceptState });

        return { startState, acceptState };
    }

    return baseFrag;
}

/**
 * <lett> ::= a | b | ... | z
 * Parses a letter (<lett>) from the token stream. Valid letters are tokens
 * of type TokenType::Letter.
 *
 * @return A Fragment representing the parsed letter's start and accept states.
 *
 * @throws std::runtime_error If the current token is not a letter.
 */
Fragment Parser::parseLetter() {
    if (currentToken().type != TokenType::Letter) {
        throw std::runtime_error("Expected a letter at position " + std::to_string(currentIndex));
    }

    int startState = stateCounter++;
    int acceptState = stateCounter++;

    // Direct transition on the letter from start to accept state
    automaton.push_back({ startState, currentToken().value, acceptState, -1 });

    advanceToken(); // Consume the letter

    return { startState, acceptState };
}

/**
 * Retrieves the constructed automaton as a vector of AutomatonState objects.
 *
 * @return A const reference to the vector of AutomatonState objects
 *         representing the constructed automaton.
 */
const std::vector<AutomatonState>& Parser::getAutomaton() const {
    return automaton;
}

/**
 * Prints the constructed automaton to the specified output stream.
 *
 * @param out The output stream where the automaton will be printed.
 */
void Parser::printAutomaton(std::ostream& out) const {
    for (const auto& state : automaton) {
        out << state.toString() << "\n";
    }
}
