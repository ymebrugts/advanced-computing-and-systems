/**
 * @author Yme Brugts (s4536622)
 * @file parser.cpp
 * @date 2024-10-09
 */

#include "parser.h"
#include "../nodes/operator_node.h"
#include "../nodes/constant_node.h"
#include "../nodes/variable_node.h"
#include "../utils/string_utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

/**
 * @function parseExpression
 * @abstract Parses a queue of tokens representing a prefix expression into an expression tree.
 * @param tokens The queue (`std::queue<std::string>&`) containing tokens to be parsed.
 * @return A shared pointer (`std::shared_ptr<ExpressionNode>`) to the root of the parsed expression tree.
 * @pre The `tokens` queue must contain a valid prefix expression with tokens in the correct order.
 * @post - The function returns a fully constructed expression tree based on the provided tokens.
 *       - The `tokens` queue is modified by removing tokens that have been parsed.
 **/
std::shared_ptr<ExpressionNode> parseExpression(std::queue<std::string>& tokens) {
    if (tokens.empty()) {
        throw std::runtime_error("Unexpected end of input");
    }
    std::string token = tokens.front();
    tokens.pop();

    if (token == "+" || token == "-" || token == "*" || token == "/") {
        auto node = std::make_shared<OperatorNode>(token);
        node->operands.push_back(parseExpression(tokens));
        node->operands.push_back(parseExpression(tokens));
        return node;
    }
    else if (token == "^") {
        auto node = std::make_shared<OperatorNode>(token);
        node->operands.push_back(parseExpression(tokens));

        // Exponent should only be a number -> A constant_node. 
        auto exponentNode = parseExpression(tokens);
        if (std::dynamic_pointer_cast<ConstantNode>(exponentNode) == nullptr) {
            throw std::runtime_error("Exponent must be a constant number");
        }

        node->operands.push_back(exponentNode);
        return node;
    }
    else if (token == "sin" || token == "cos" || token == "exp" || token == "log") {
        auto node = std::make_shared<OperatorNode>(token);
        node->operands.push_back(parseExpression(tokens));
        return node;
    }
    else if (is_number(token) || (token[0] == 'p' && token[1] == 'i')) {
        return std::make_shared<ConstantNode>(token);
    }
    else {
        return std::make_shared<VariableNode>(token);
    }
}