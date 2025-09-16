/**
 * classname: ConstantNode - Represents a constant value in the expression tree and handles its infix printing and DOT representation.
 * author Yme Brugts (s4536622)
 * @file constant_node.cpp
 * @date 2024-10-09
 */

#include "constant_node.h"
#include <ostream>
#include <string>
#include <cmath>
#include "../utils/double_utils.h"

constexpr double EPSILON = 1e-10;
constexpr double PRECISIONOUTPUT = 1e-5;
constexpr double PI = 3.14159265358979323846;
const std::string PI_S = "3.14159265358979323846";

ConstantNode::ConstantNode(const std::string& value) {
    if (value == "pi" || value == PI_S) {
        this->value = "pi";
        return;
    }

    // Round the value based on epsilon
    double doubleValue = stod(value);
    double roundedValue = std::round(doubleValue / EPSILON) * EPSILON;

    if (std::abs(roundedValue - PI) <= EPSILON) {
        this->value = "pi";
        return;
    }

    // Check for zero or one within epsilon
    if (std::abs(roundedValue - 0.0) <= EPSILON) {
        isValueZero = true;
        roundedValue = 0.0;
    }
    else if (std::abs(roundedValue - 1.0) <= EPSILON) {
        isValueOne = true;
        roundedValue = 1.0;
    }

    this->value = formatDouble(roundedValue);
}


/**
 * @function printInfix
 * @abstract Prints the constant value in infix notation to the provided output stream.
 * @param out The output stream (`std::ostream&`) where the infix notation is written.
 * @param parentPrecedence The precedence level of the parent node (`int`) to determine if parentheses are needed.
 * @pre The output stream `out` must be open and ready for writing.
 * @post The constant value is written to the output stream `out`.
 **/
void ConstantNode::printInfix(std::ostream& out,int parentPrecedence) {
    if (value == "pi") {
        out << value;
        return;
    }
    double doubleValue = stod(value);
    double roundedValue = std::round(doubleValue / PRECISIONOUTPUT) * PRECISIONOUTPUT;
    out << roundedValue;
}

/**
 * @function isOperator
 * @abstract Determines whether the node is an operator.
 * @return A boolean (`bool`) value `false` indicating that ConstantNode is not an operator.
 **/
bool ConstantNode::isOperator() const {
    return false;
}

/**
 * @function getNodePrecedence
 * @abstract Retrieves the precedence level of the node.
 * @return An integer (`int`) representing the precedence level, which is `0` for ConstantNode.
 **/
int ConstantNode::getNodePrecedence() const {
    return 0;
}

/**
 * @function writeDot
 * @abstract Writes the DOT representation of the constant node to the provided output stream for Graphviz visualization.
 * @param out The output stream (`std::ostream&`) where the DOT notation is written.
 * @param nextId A reference to an integer (`int&`) used to assign a unique ID to the node.
 * @return An integer (`int`) representing the unique ID assigned to the constant node.
 * @pre - The output stream `out` must be open and ready for writing.
 *      - The reference `nextId` must be a valid integer.
 * @post - The DOT representation of the constant node is written to the output stream `out`.
 *       - The `nextId` is incremented to ensure uniqueness for subsequent nodes.
 **/
int ConstantNode::writeDot(std::ostream& out, int& nextId) {
    int currentId = nextId++;  // Assign a unique ID to the constant node
    if (value == "pi") {
        out << currentId << " [label=\"" << "&pi;" << "\"];" << std::endl;
        return currentId;
    }
    out << currentId << " [label=\"" << value << "\"];" << std::endl;
    return currentId;  // Return the ID
}

void ConstantNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

/**
 * @function isZero
 * @abstract Determines whether the node has value zero
 * @return A boolean (`bool`) value indicating whether the constant node is zero
 **/
bool ConstantNode::isZero() {
    return isValueZero;
}

/**
 * @function isOne
 * @abstract Determines whether the node has value One
 * @return A boolean (`bool`) value indicating whether the constant node is One
 **/
bool ConstantNode::isOne() {
    return isValueOne;
}


/**
 * @function getValueAsDouble
 * @abstract Returns the value as double or a symbolic type as double
 * @return A double which represents the value of the constant node
 **/
double ConstantNode::getValueAsDouble() const {
    try {
        if (value == "pi") {
            return PI;
        }
        return std::stod(value);
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid constant value for conversion to double: " + value);
    }
}

