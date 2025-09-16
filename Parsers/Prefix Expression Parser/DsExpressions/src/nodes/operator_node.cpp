/**
 * classname: OperatorNode - Represents an operator in the expression tree, handling precedence, infix printing, and DOT representation.
 * author Yme Brugts (s4536622)
 * @file operator_node.cpp
 * @date 2024-10-09
 */

#include "operator_node.h"
#include <ostream>
#include <vector>
#include "variable_node.h"
#include "constant_node.h"

OperatorNode::OperatorNode(const std::string& op) : op(op) {}

/**
 * @function getPrecedence
 * @abstract Determines the precedence level of a given operator.
 * @param op The operator (`const std::string&`) whose precedence is to be determined.
 * @return An integer representing the precedence level of the operator.
 * @pre The input string `op` must be a valid operator such as "sin", "cos", "^", "*", "/", "+", or "-".
 * @post Returns the corresponding precedence level:
 *       - 4 for "sin", "cos", "exp"
 *       - 3 for "^"
 *       - 2 for "*" and "/"
 *       - 1 for "+" and "-"
 *       - 0 for unrecognized operators, numbers, and variables.
 **/
int getPrecedence(const std::string& op) {
    if (op == "sin" || op == "cos" || op == "exp") {
        return 4;  // Highest precedence for functions
    }
    else if (op == "^") {
        return 3;
    }
    else if (op == "*" || op == "/") {
        return 2;
    }
    else if (op == "+" || op == "-") {
        return 1;
    }
    else {
        return 0;  // For numbers and variables
    }
}

/**
 * @function isOperator
 * @abstract Determines whether the node is an operator.
 * @return A boolean value `true` indicating that OperatorNode is an operator.
 **/
bool OperatorNode::isOperator() const {
    return true;
}

/**
 * @function getNodePrecedence
 * @abstract Retrieves the precedence level of the operator node.
 * @return An integer representing the precedence level of the operator.
 **/
int OperatorNode::getNodePrecedence() const {
    return getPrecedence(op);
}

/**
 * @function printInfix
 * @abstract Prints the operator and its operands in infix notation to the provided output stream.
 * @param out The output stream (`std::ostream&`) where the infix notation is written.
 * @param parentPrecedence The precedence level of the parent node (`int`) to determine if parentheses are needed.
 * @pre - The output stream `out` must be open and ready for writing.
 *      - The `operands` vector must contain the correct number of operand nodes for the operator.
 * @post - The operator and its operands are written in infix notation to the output stream `out`.
 *       - Parentheses are added based on precedence rules to maintain correct expression evaluation order.
 **/
void OperatorNode::printInfix(std::ostream& out, int parentPrecedence) {
    int currentPrecedence = getPrecedence(op);

    if (operands.size() == 1) {
        // Unary operator (e.g., sin, cos)
        out << op << "(";
        operands[0]->printInfix(out, currentPrecedence);
        out << ")";
    }
    else if (operands.size() == 2) {
        // Determine if parentheses are needed around the entire expression
        bool isParentFunction = (parentPrecedence == getPrecedence("sin") || parentPrecedence == getPrecedence("cos") || parentPrecedence == getPrecedence("exp"));
        bool needParens = (currentPrecedence < parentPrecedence) && !isParentFunction;

        if (needParens) {
            out << "(";
        }

        // Left operand
        operands[0]->printInfix(out, currentPrecedence);

        // Operator
        out << op;

        // Right operand
        operands[1]->printInfix(out, currentPrecedence);
        if (needParens) {
            out << ")";
        }
    }
}


/**
 * @function generateNodeId
 * @abstract Generates a unique node ID for DOT graph representation.
 * @return An integer (`int`) representing the unique node ID.
 * @post Incremented a static counter.
 **/
int generateNodeId() {
    static int nodeId = 0;
    return ++nodeId;
}

/**
 * @function writeDot
 * @abstract Writes the DOT representation of the operator node and its operands to the provided output stream for Graphviz visualization.
 * @param out The output stream (`std::ostream&`) where the DOT notation is written.
 * @param nextId A reference to an integer (`int&`) used to assign a unique ID to the node.
 * @return An integer (`int`) representing the unique ID assigned to the operator node.
 * @pre - The output stream `out` must be open and ready for writing.
 *      - The reference `nextId` must be a valid integer.
 *      - The `operands` vector must contain the correct operand nodes.
 * @post - The DOT representation of the operator node and its connections to operands is written to the output stream `out`.
 *       - The `nextId` is incremented to ensure uniqueness for subsequent nodes.
 **/
int OperatorNode::writeDot(std::ostream& out, int& nextId) {
    int currentId = nextId++;  // Assign a unique ID to the current node
    out << currentId << " [label=\"" << op << "\"];" << std::endl;

    // Iterate over operands and create edges to each child
    for (const auto& child : operands) {
        // Recursively write the child node and get its unique ID
        int childId = child->writeDot(out, nextId);

        // Connect the parent node to the child node
        out << currentId << " -> " << childId << ";" << std::endl;
    }

    return currentId;  // Return the current node's ID to the parent
}

/**
 * @function accept
 * @abstract Accepts a visitor to perform operations on the OperatorNode.
 * @param visitor The visitor object that implements operations on the node.
 */
void OperatorNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}



