/**
 * author Yme Brugts (s4536622)
 * @file eval_visitor.cpp
 * @date 2024-10-28
 */

#include "eval_visitor.h"

EvalVisitor::EvalVisitor(const std::string& varName, const std::string& varValue)
    : variableName(varName), variableValue(varValue) {}

/**
 * @function visit
 * @abstract Visits a ConstantNode and sets the evaluation result to the constant's value.
 * @param node The ConstantNode being visited.
 */
void EvalVisitor::visit(ConstantNode& node) {
    // Constants remain unchanged
    result = std::make_shared<ConstantNode>(node.value);
}

/**
 * @function visit
 * @abstract Visits a VariableNode and substitutes it with the provided value if it matches the target variable.
 * @param node The VariableNode being visited.
 */
void EvalVisitor::visit(VariableNode& node) {
    if (node.identifier == variableName) {
        // Replace variable with the provided value
        result = std::make_shared<ConstantNode>(variableValue);
    }
    else {
        // Leave other variables unchanged
        result = std::make_shared<VariableNode>(node.identifier);
    }
}

/**
 * @function visit
 * @abstract Visits an OperatorNode, evaluates its operands, and reconstructs the operator node with evaluated operands.
 * @param node The OperatorNode being visited.
 */
void EvalVisitor::visit(OperatorNode& node) {
    // Evaluate operands first
    node.operands[0]->accept(*this);
    auto leftEvaluated = result;

    std::shared_ptr<ExpressionNode> rightEvaluated;
    if (node.operands.size() > 1 && node.operands[1]) {
        node.operands[1]->accept(*this);
        rightEvaluated = result;
    }

    // Reconstruct the operator node with evaluated operands
    auto newNode = std::make_shared<OperatorNode>(node.op);
    newNode->operands.push_back(leftEvaluated);
    if (rightEvaluated) {
        newNode->operands.push_back(rightEvaluated);
    }

    result = newNode;
}