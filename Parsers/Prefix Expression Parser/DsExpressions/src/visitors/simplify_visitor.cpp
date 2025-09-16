/**
 * author Yme Brugts (s4536622)
 * @file simplify_visitor.cpp
 * @date 2024-10-27
 */

#include "simplify_visitor.h"
#include "string"
#include <cmath>
#include "../utils/string_utils.h"

SimplifyVisitor::SimplifyVisitor() {}

/**
 * @function visit
 * @abstract Visits a ConstantNode and sets the simplification result to the constant's value.
 * @param node The ConstantNode being visited.
 */
void SimplifyVisitor::visit(ConstantNode& node) {
    result = std::make_shared<ConstantNode>(node.value);
}

/**
 * @function visit
 * @abstract Visits a VariableNode and sets the simplification result to the variable itself.
 * @param node The VariableNode being visited.
 */
void SimplifyVisitor::visit(VariableNode& node) {
    result = std::make_shared<VariableNode>(node.identifier);
}

/**
 * @function visit
 * @abstract Visits an OperatorNode, simplifies its operands, and applies the appropriate simplification rule.
 * @param node The OperatorNode being visited.
 */
void SimplifyVisitor::visit(OperatorNode& node) {
    // Simplify operands first
    node.operands[0]->accept(*this);
    auto leftSimplified = result;

    std::shared_ptr<ExpressionNode> rightSimplified;
    if (node.operands.size() > 1 && node.operands[1]) {
        node.operands[1]->accept(*this);
        rightSimplified = result;
    }

    if (node.op == "+") {
        result = simplifyAddition(leftSimplified, rightSimplified);
    }
    else if (node.op == "-") {
        result = simplifySubtraction(leftSimplified, rightSimplified);
    }
    else if (node.op == "*") {
        result = simplifyMultiplication(leftSimplified, rightSimplified);
    }
    else if (node.op == "/") {
        result = simplifyDivision(leftSimplified, rightSimplified);
    }
    else if (node.op == "^") {
        result = simplifyExponentiation(leftSimplified, rightSimplified);
    }
    else if (node.op == "sin" || node.op == "cos" || node.op == "exp" || node.op == "log") {
        result = simplifyUnaryOperation(node.op, leftSimplified);
    }
    else {
        throw std::runtime_error("Unrecognized operator during simplification");
    }
}

/**
 * @function areEquivalent
 * @abstract Checks if two expression nodes are equivalent based on their type and value.
 * @param left The first expression node to compare.
 * @param right The second expression node to compare.
 * @return A boolean indicating whether the two nodes are equivalent.
 */
bool SimplifyVisitor::areEquivalent(const std::shared_ptr<ExpressionNode>& left,
    const std::shared_ptr<ExpressionNode>& right) {
    // Simple equivalence check based on node type and value
    if (auto constLeft = std::dynamic_pointer_cast<ConstantNode>(left)) {
        if (auto constRight = std::dynamic_pointer_cast<ConstantNode>(right)) {
            return constLeft->value == constRight->value;
        }
    }
    else if (auto varLeft = std::dynamic_pointer_cast<VariableNode>(left)) {
        if (auto varRight = std::dynamic_pointer_cast<VariableNode>(right)) {
            return varLeft->identifier == varRight->identifier;
        }
    }
    return false;
}

/**
 * @function simplifyAddition
 * @abstract Simplifies an addition operation by applying constant folding and identity rules.
 * @param left The left operand of the addition.
 * @param right The right operand of the addition.
 * @return A simplified ExpressionNode representing the addition.
 */
std::shared_ptr<ExpressionNode> SimplifyVisitor::simplifyAddition(
    std::shared_ptr<ExpressionNode> left,
    std::shared_ptr<ExpressionNode> right) {

    if (auto leftConst = std::dynamic_pointer_cast<ConstantNode>(left)) {
        if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
            // Handle addition both sides
            double sum = leftConst->getValueAsDouble() + rightConst->getValueAsDouble();
            return createConstant(sum);
        }
    }

    if (auto leftConst = std::dynamic_pointer_cast<ConstantNode>(left)) {
        // Handle addition with zero
        if (leftConst->isZero()) {
            return right;
        }
    }

    if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
        // Handle addition with zero
        if (rightConst->isZero()) {
            return left;
        }
    }
    // No simplification possible, reconstruct the node
    auto node = std::make_shared<OperatorNode>("+");
    node->operands.push_back(left);
    node->operands.push_back(right);
    return node;
}

/**
 * @function simplifySubtraction
 * @abstract Simplifies a subtraction operation by applying constant folding and identity rules.
 * @param left The left operand of the subtraction.
 * @param right The right operand of the subtraction.
 * @return A simplified ExpressionNode representing the subtraction.
 */
std::shared_ptr<ExpressionNode> SimplifyVisitor::simplifySubtraction(
    std::shared_ptr<ExpressionNode> left,
    std::shared_ptr<ExpressionNode> right) {

    // If both operands are constants, compute the difference
    if (auto leftConst = std::dynamic_pointer_cast<ConstantNode>(left)) {
        if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
            double difference = leftConst->getValueAsDouble() - rightConst->getValueAsDouble();
            return createConstant(difference);
        }
    }

    // Subtracting zero
    if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
        if (rightConst->isZero()) {
            return left;
        }
    }

    // Simplify x - x = 0 (assuming x is not 0)
    if (areEquivalent(left, right)) {
        return createConstant(0);
    }


    // No simplification possible, reconstruct the node
    auto node = std::make_shared<OperatorNode>("-");
    node->operands.push_back(left);
    node->operands.push_back(right);
    return node;
}

/**
 * @function simplifyMultiplication
 * @abstract Simplifies a multiplication operation by applying constant folding and identity rules.
 * @param left The left operand of the multiplication.
 * @param right The right operand of the multiplication.
 * @return A simplified ExpressionNode representing the multiplication.
 */
std::shared_ptr<ExpressionNode> SimplifyVisitor::simplifyMultiplication(
    std::shared_ptr<ExpressionNode> left,
    std::shared_ptr<ExpressionNode> right) {

    if (auto leftConst = std::dynamic_pointer_cast<ConstantNode>(left)) {
        if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
            double product = leftConst->getValueAsDouble() * rightConst->getValueAsDouble();
            return createConstant(product);
        }
        // Multiplying by zero
        if (leftConst->isZero()) {
            return createConstant(0);
        }
        // Multiplying by one
        else if (leftConst->isOne()) {
            return right;
        }
    }

    if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
        // Multiplying by zero
        if (rightConst->isZero()) {
            return createConstant(0);
        }
        // Multiplying by one
        else if (rightConst->isOne()) {
            return left;
        }
    }

    // No simplification possible, reconstruct the node
    auto node = std::make_shared<OperatorNode>("*");
    node->operands.push_back(left);
    node->operands.push_back(right);
    return node;
}

/**
 * @function simplifyDivision
 * @abstract Simplifies a division operation by applying constant folding and identity rules.
 * @param left The numerator of the division.
 * @param right The denominator of the division.
 * @return A simplified ExpressionNode representing the division.
 * @throws std::runtime_error if division by zero is detected.
 */
std::shared_ptr<ExpressionNode> SimplifyVisitor::simplifyDivision(
    std::shared_ptr<ExpressionNode> left,
    std::shared_ptr<ExpressionNode> right) {

    // If both operands are constants, compute the quotient
    if (auto leftConst = std::dynamic_pointer_cast<ConstantNode>(left)) {
        if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
            if (rightConst->isZero()) {
                throw std::runtime_error("Division by zero");
            }
            double quotient = leftConst->getValueAsDouble() / rightConst->getValueAsDouble();
            return createConstant(quotient);
        }
    }

    // Dividing zero by any value
    if (auto leftConst = std::dynamic_pointer_cast<ConstantNode>(left)) {
        if (leftConst->isZero()) {
            return createConstant(0);
        }
    }

    // Dividing by one
    if (auto rightConst = std::dynamic_pointer_cast<ConstantNode>(right)) {
        if (rightConst->isOne()) {
            return left;
        }
        if (rightConst->isZero()) {
            throw std::runtime_error("Division by zero");
        }
    }

    // Simplify x / x = 1 (assuming x is not 0)
    if (areEquivalent(left, right)) {
        return createConstant(1);
    }

    // No simplification possible, reconstruct the node
    auto node = std::make_shared<OperatorNode>("/");
    node->operands.push_back(left);
    node->operands.push_back(right);
    return node;
}

/**
 * @function simplifyExponentiation
 * @abstract Simplifies an exponentiation operation by applying constant folding and identity rules.
 * @param base The base of the exponentiation.
 * @param exponent The exponent of the exponentiation.
 * @return A simplified ExpressionNode representing the exponentiation.
 * @throws std::runtime_error if an indeterminate form or unsupported operation is detected.
 */
std::shared_ptr<ExpressionNode> SimplifyVisitor::simplifyExponentiation(
    std::shared_ptr<ExpressionNode> base,
    std::shared_ptr<ExpressionNode> exponent) {

    // If both base and exponent are constants, compute the power
    if (auto baseConst = std::dynamic_pointer_cast<ConstantNode>(base)) {
        if (auto exponentConst = std::dynamic_pointer_cast<ConstantNode>(exponent)) {

            if (baseConst->value == "0" && exponentConst->value == "0") {
                throw std::runtime_error("Indeterminate form 0^0");
            }

            double power = std::pow(baseConst->getValueAsDouble(), exponentConst->getValueAsDouble());
            return createConstant(power);
        }
    }

    // Exponent is zero
    if (auto exponentConst = std::dynamic_pointer_cast<ConstantNode>(exponent)) {
        if (exponentConst->isZero()) {
            return createConstant(1);
        }
        else if (exponentConst->isOne()) {
            return base;
        }
    }

    // Base is one
    if (auto baseConst = std::dynamic_pointer_cast<ConstantNode>(base)) {
        if (baseConst->isOne()) {
            return createConstant(1);
        }
        else if (baseConst->isZero()) {
            return createConstant(0);
        }
    }

    // No simplification possible, reconstruct the node
    auto node = std::make_shared<OperatorNode>("^");
    node->operands.push_back(base);
    node->operands.push_back(exponent);
    return node;
}

/**
 * @function simplifyUnaryOperation
 * @abstract Simplifies a unary operation by applying constant folding if possible.
 * @param op The unary operator as a string (e.g., "sin", "cos", "exp", "log").
 * @param operand The operand of the unary operation.
 * @return A simplified ExpressionNode representing the unary operation.
 * @throws std::runtime_error if an unsupported operator or invalid operation is detected.
 */
std::shared_ptr<ExpressionNode> SimplifyVisitor::simplifyUnaryOperation(
    const std::string& op,
    std::shared_ptr<ExpressionNode> operand) {

    // If the operand is a constant, compute the function
    if (auto operandConst = std::dynamic_pointer_cast<ConstantNode>(operand)) {
        double value = operandConst->getValueAsDouble();
        double resultValue;

        if (op == "sin") {
            resultValue = std::sin(value);
        }
        else if (op == "cos") {
            resultValue = std::cos(value);
        }
        else if (op == "exp") {
            resultValue = std::exp(value);
        }
        else if (op == "log") {
            if (value <= 0) {
                throw std::runtime_error("Logarithm of non-positive number");
            }
            resultValue = std::log(value);
        }
        else {
            throw std::runtime_error("Unrecognized operator during simplification of unary operation");
        }

        return createConstant(resultValue);
    }

    // No simplification possible, reconstruct the node
    auto node = std::make_shared<OperatorNode>(op);
    node->operands.push_back(operand);
    return node;
}

/**
 * @function createConstant
 * @abstract Creates a ConstantNode from a double value, ensuring full precision.
 * @param value The double value to convert into a ConstantNode.
 * @return A shared pointer to the newly created ConstantNode.
 */
std::shared_ptr<ConstantNode> SimplifyVisitor::createConstant(const double& value) {
    return std::make_shared<ConstantNode>(toFullPrecisionString(value));
}



