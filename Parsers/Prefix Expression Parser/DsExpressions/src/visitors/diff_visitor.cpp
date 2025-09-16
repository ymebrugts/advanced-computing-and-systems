/**
 * author Yme Brugts (s4536622)
 * @file diff_visitor.cpp
 * @date 2024-10-28
 */

#include "diff_visitor.h"

DiffVisitor::DiffVisitor(const std::string& var) : variable(var) {}

/**
 * @function visit
 * @abstract Visits a ConstantNode and sets the derivative to zero.
 * @param node The ConstantNode being visited.
 * @post The result is set to a ConstantNode with value "0".
 */
void DiffVisitor::visit(ConstantNode& node) {
    // Derivative of a constant is zero
    result = std::make_shared<ConstantNode>("0");
}

/**
 * @function visit
 * @abstract Visits a VariableNode and computes its derivative.
 * @param node The VariableNode being visited.
 * @post The result is set to "1" if the variable matches, else "0".
 */
void DiffVisitor::visit(VariableNode& node) {
    if (node.identifier == variable) {
        // Derivative of the variable with respect to itself is 1
        result = std::make_shared<ConstantNode>("1");
    }
    else {
        // Derivative of other variables is zero
        result = std::make_shared<ConstantNode>("0");
    }
}

/**
 * @function visit
 * @abstract Visits an OperatorNode and applies the appropriate differentiation rule.
 * @param node The OperatorNode being visited.
 * @post The result contains the differentiated expression.
 */
void DiffVisitor::visit(OperatorNode& node) {
    // Differentiate operands first
    node.operands[0]->accept(*this);
    auto leftDerivative = result;

    std::shared_ptr<ExpressionNode> rightDerivative;
    if (node.operands.size() > 1 && node.operands[1]) {
        node.operands[1]->accept(*this);
        rightDerivative = result;
    }

    if (node.op == "+") {
        result = differentiateAddition(node.operands[0], node.operands[1], leftDerivative, rightDerivative);
    }
    else if (node.op == "-") {
        result = differentiateSubtraction(node.operands[0], node.operands[1], leftDerivative, rightDerivative);
    }
    else if (node.op == "*") {
        result = differentiateMultiplication(node.operands[0], node.operands[1], leftDerivative, rightDerivative);
    }
    else if (node.op == "/") {
        result = differentiateDivision(node.operands[0], node.operands[1], leftDerivative, rightDerivative);
    }
    else if (node.op == "^") {
        result = differentiateExponentiation(node.operands[0], node.operands[1]);
    }
    else if (node.op == "sin" || node.op == "cos" || node.op == "exp" || node.op == "log") {
        result = differentiateUnaryOperation(node.op, node.operands[0], leftDerivative);
    }
    else {
        throw std::runtime_error("Unsupported operator in differentiation: " + node.op);
    }
}

/**
 * @function differentiateAddition
 * @abstract Differentiates an addition operation using the sum rule.
 * @param leftOperand The left operand of the addition.
 * @param rightOperand The right operand of the addition.
 * @param leftDerivative The derivative of the left operand.
 * @param rightDerivative The derivative of the right operand.
 * @return A new ExpressionNode representing the derivative.
 */
std::shared_ptr<ExpressionNode> DiffVisitor::differentiateAddition(
    const std::shared_ptr<ExpressionNode>& leftOperand,
    const std::shared_ptr<ExpressionNode>& rightOperand,
    const std::shared_ptr<ExpressionNode>& leftDerivative,
    const std::shared_ptr<ExpressionNode>& rightDerivative) {

    // Sum Rule: derivative of sum is the sum of derivatives
    auto newNode = std::make_shared<OperatorNode>("+");
    newNode->operands.push_back(leftDerivative);
    newNode->operands.push_back(rightDerivative);
    return newNode;
}

/**
 * @function differentiateSubtraction
 * @abstract Differentiates a subtraction operation using the difference rule.
 * @param leftOperand The left operand of the subtraction.
 * @param rightOperand The right operand of the subtraction.
 * @param leftDerivative The derivative of the left operand.
 * @param rightDerivative The derivative of the right operand.
 * @return A new ExpressionNode representing the derivative.
 */
std::shared_ptr<ExpressionNode> DiffVisitor::differentiateSubtraction(
    const std::shared_ptr<ExpressionNode>& leftOperand,
    const std::shared_ptr<ExpressionNode>& rightOperand,
    const std::shared_ptr<ExpressionNode>& leftDerivative,
    const std::shared_ptr<ExpressionNode>& rightDerivative) {

    // Difference Rule: derivative of difference is the difference of derivatives
    auto newNode = std::make_shared<OperatorNode>("-");
    newNode->operands.push_back(leftDerivative);
    newNode->operands.push_back(rightDerivative);
    return newNode;
}


/**
 * @function differentiateMultiplication
 * @abstract Differentiates a multiplication operation using the product rule.
 * @param leftOperand The left operand of the multiplication.
 * @param rightOperand The right operand of the multiplication.
 * @param leftDerivative The derivative of the left operand.
 * @param rightDerivative The derivative of the right operand.
 * @return A new ExpressionNode representing the derivative.
 */
std::shared_ptr<ExpressionNode> DiffVisitor::differentiateMultiplication(
    const std::shared_ptr<ExpressionNode>& leftOperand,
    const std::shared_ptr<ExpressionNode>& rightOperand,
    const std::shared_ptr<ExpressionNode>& leftDerivative,
    const std::shared_ptr<ExpressionNode>& rightDerivative) {

    // Product Rule: (f * g)' = f' * g + f * g'
    // f' * g
    auto term1 = std::make_shared<OperatorNode>("*");
    term1->operands.push_back(leftDerivative);
    term1->operands.push_back(rightOperand);

    // f * g'
    auto term2 = std::make_shared<OperatorNode>("*");
    term2->operands.push_back(leftOperand);
    term2->operands.push_back(rightDerivative);

    // Sum: f' * g + f * g'
    auto newNode = std::make_shared<OperatorNode>("+");
    newNode->operands.push_back(term1);
    newNode->operands.push_back(term2);
    return newNode;
}

/**
 * @function differentiateDivision
 * @abstract Differentiates a division operation using the quotient rule.
 * @param leftOperand The numerator of the division.
 * @param rightOperand The denominator of the division.
 * @param leftDerivative The derivative of the numerator.
 * @param rightDerivative The derivative of the denominator.
 * @return A new ExpressionNode representing the derivative.
 */
std::shared_ptr<ExpressionNode> DiffVisitor::differentiateDivision(
    const std::shared_ptr<ExpressionNode>& leftOperand,
    const std::shared_ptr<ExpressionNode>& rightOperand,
    const std::shared_ptr<ExpressionNode>& leftDerivative,
    const std::shared_ptr<ExpressionNode>& rightDerivative) {

    // Quotient Rule: (f / g)' = (f' * g - f * g') / g^2
    // f' * g
    auto numeratorTerm1 = std::make_shared<OperatorNode>("*");
    numeratorTerm1->operands.push_back(leftDerivative);
    numeratorTerm1->operands.push_back(rightOperand);

    // f * g'
    auto numeratorTerm2 = std::make_shared<OperatorNode>("*");
    numeratorTerm2->operands.push_back(leftOperand);
    numeratorTerm2->operands.push_back(rightDerivative);

    // Numerator: f' * g - f * g'
    auto numerator = std::make_shared<OperatorNode>("-");
    numerator->operands.push_back(numeratorTerm1);
    numerator->operands.push_back(numeratorTerm2);

    // Denominator: g^2
    auto exponentTwo = std::make_shared<ConstantNode>("2");
    auto denominator = std::make_shared<OperatorNode>("^");
    denominator->operands.push_back(rightOperand);
    denominator->operands.push_back(exponentTwo);

    // Quotient: (f' * g - f * g') / g^2
    auto newNode = std::make_shared<OperatorNode>("/");
    newNode->operands.push_back(numerator);
    newNode->operands.push_back(denominator);
    return newNode;
}

/**
 * @function differentiateExponentiation
 * @abstract Differentiates an exponentiation operation where the exponent is a constant.
 * @param base The base of the exponentiation.
 * @param exponent The exponent of the exponentiation.
 * @return A new ExpressionNode representing the derivative.
 * @throws std::runtime_error if the exponent is not a constant.
 */
std::shared_ptr<ExpressionNode> DiffVisitor::differentiateExponentiation(
    const std::shared_ptr<ExpressionNode>& base,
    const std::shared_ptr<ExpressionNode>& exponent) {

    // Handle u^C where C is a constant
    if (auto exponentConst = std::dynamic_pointer_cast<ConstantNode>(exponent)) {
        // Derivative: C * u^(C - 1) * u'
        double exponentValue = exponentConst->getValueAsDouble();
        double newExponentValue = exponentValue - 1;

        auto newExponentNode = std::make_shared<ConstantNode>(std::to_string(newExponentValue));

        // u^(C - 1)
        auto powerNode = std::make_shared<OperatorNode>("^");
        powerNode->operands.push_back(base);
        powerNode->operands.push_back(newExponentNode);

        // C * u^(C - 1)
        auto coefficientNode = std::make_shared<ConstantNode>(exponentConst->value);
        auto term = std::make_shared<OperatorNode>("*");
        term->operands.push_back(coefficientNode);
        term->operands.push_back(powerNode);

        // Compute u'
        base->accept(*this);
        auto baseDerivative = result;

        // Multiply: C * u^(C - 1) * u'
        auto newNode = std::make_shared<OperatorNode>("*");
        newNode->operands.push_back(term);
        newNode->operands.push_back(baseDerivative);
        return newNode;
    }
    else {
        // Exponent is not constant; general power rule requires more complex handling
        throw std::runtime_error("Differentiation of variable exponents is not supported.");
    }
}

/**
 * @function differentiateUnaryOperation
 * @abstract Differentiates unary operations (sin, cos, exp, log) using the chain rule.
 * @param op The operator as a string.
 * @param operand The operand of the unary operation.
 * @param operandDerivative The derivative of the operand.
 * @return A new ExpressionNode representing the derivative.
 * @throws std::runtime_error if the operator is unsupported.
 */
std::shared_ptr<ExpressionNode> DiffVisitor::differentiateUnaryOperation(
    const std::string& op,
    const std::shared_ptr<ExpressionNode>& operand,
    const std::shared_ptr<ExpressionNode>& operandDerivative) {

    if (op == "sin") {
        // Chain Rule: (sin(u))' = cos(u) * u'
        // cos(u)
        auto cosNode = std::make_shared<OperatorNode>("cos");
        cosNode->operands.push_back(operand);

        // Multiply: cos(u) * u'
        auto newNode = std::make_shared<OperatorNode>("*");
        newNode->operands.push_back(cosNode);
        newNode->operands.push_back(operandDerivative);
        return newNode;
    }
    else if (op == "cos") {
        // Chain Rule: (cos(u))' = -sin(u) * u'
        // sin(u)
        auto sinNode = std::make_shared<OperatorNode>("sin");
        sinNode->operands.push_back(operand);

        // -sin(u)
        auto negSinNode = std::make_shared<OperatorNode>("*");
        negSinNode->operands.push_back(std::make_shared<ConstantNode>("-1"));
        negSinNode->operands.push_back(sinNode);

        // Multiply: -sin(u) * u'
        auto newNode = std::make_shared<OperatorNode>("*");
        newNode->operands.push_back(negSinNode);
        newNode->operands.push_back(operandDerivative);
        return newNode;
    }
    else if (op == "exp") {
        // Chain Rule: (exp(u))' = exp(u) * u'
        // exp(u)
        auto expNode = std::make_shared<OperatorNode>("exp");
        expNode->operands.push_back(operand);

        // Multiply: exp(u) * u'
        auto newNode = std::make_shared<OperatorNode>("*");
        newNode->operands.push_back(expNode);
        newNode->operands.push_back(operandDerivative);
        return newNode;
    }
    else if (op == "log") {
        // Chain Rule: (log(u))' = u' / u
        // Divide: u' / u
        auto newNode = std::make_shared<OperatorNode>("/");
        newNode->operands.push_back(operandDerivative);
        newNode->operands.push_back(operand);
        return newNode;
    }
    else {
        throw std::runtime_error("Unsupported unary operator in differentiation: " + op);
    }
}
