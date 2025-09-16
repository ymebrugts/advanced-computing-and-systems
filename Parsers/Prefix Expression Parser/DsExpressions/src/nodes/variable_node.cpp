/**
 * classname: VariableNode - Represents a variable in the expression tree and handles its infix printing and DOT representation.
 * @author Yme Brugts (s4536622)
 * @file variable_node.cpp
 * @date 2024-10-09
 */

#include "expression_node.h"
#include <ostream>

#include "variable_node.h"


VariableNode::VariableNode(const std::string& identifier) : identifier(identifier) {}

/**
 * @function printInfix
 * @abstract Prints the variable identifier in infix notation to the provided output stream.
 * @param out The output stream (`std::ostream&`) where the infix notation is written.
 * @param parentPrecedence The precedence level of the parent node (`int`) to determine if parentheses are needed.
 * @pre The output stream `out` must be open and ready for writing.
 * @post The variable identifier is written to the output stream `out`.
 **/
void VariableNode::printInfix(std::ostream& out, int parentPrecedence) {
    out << identifier;
}

/**
 * @function isOperator
 * @abstract Determines whether the node is an operator.
 * @return A bool value `false` indicating that VariableNode is not an operator.
 **/
bool VariableNode::isOperator() const {
    return false;
}

/**
 * @function getNodePrecedence
 * @abstract Retrieves the precedence level of the node.
 * @return An integer (`int`) representing the precedence level, which is `0` for VariableNode.
 **/
int VariableNode::getNodePrecedence() const {
    return 0;
}

/**
 * @function writeDot
 * @abstract Writes the DOT representation of the variable node to the provided output stream for Graphviz visualization.
 * @param out The output stream (`std::ostream&`) where the DOT notation is written.
 * @param nextId A reference to an integer (`int&`) used to assign a unique ID to the node.
 * @return An integer (`int`) representing the unique ID assigned to the variable node.
 * @pre - The output stream `out` must be open and ready for writing.
 *      - The reference `nextId` must be a valid integer.
 * @post - The DOT representation of the variable node is written to the output stream `out`.
 *       - The `nextId` is incremented to ensure uniqueness for subsequent nodes.
 **/
int VariableNode::writeDot(std::ostream& out, int& nextId) {
    int currentId = nextId++;  // Assign a unique ID to the variable node
    out << currentId << " [label=\"" << identifier << "\"];" << std::endl;
    return currentId;  // Return the ID
}

/**
 * @function accept
 * @abstract Accepts a visitor to perform operations on the VariableNode.
 * @param visitor The visitor object that implements operations on the node.
 */
void VariableNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}