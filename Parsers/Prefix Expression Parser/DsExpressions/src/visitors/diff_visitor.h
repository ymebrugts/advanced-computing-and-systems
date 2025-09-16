#pragma once

#include <memory>
#include "visitor.h"

#include "../nodes/constant_node.h"
#include "../nodes/variable_node.h"
#include "../nodes/operator_node.h"

class DiffVisitor : public Visitor {
public:
    std::shared_ptr<ExpressionNode> result;
    std::string variable;

    DiffVisitor(const std::string& var);

    void visit(ConstantNode& node) override;
    void visit(VariableNode& node) override;
    void visit(OperatorNode& node) override;

private:
    std::shared_ptr<ExpressionNode> differentiateAddition(
        const std::shared_ptr<ExpressionNode>& leftOperand,
        const std::shared_ptr<ExpressionNode>& rightOperand,
        const std::shared_ptr<ExpressionNode>& leftDerivative,
        const std::shared_ptr<ExpressionNode>& rightDerivative);

    std::shared_ptr<ExpressionNode> differentiateSubtraction(
        const std::shared_ptr<ExpressionNode>& leftOperand,
        const std::shared_ptr<ExpressionNode>& rightOperand,
        const std::shared_ptr<ExpressionNode>& leftDerivative,
        const std::shared_ptr<ExpressionNode>& rightDerivative);

    std::shared_ptr<ExpressionNode> differentiateMultiplication(
        const std::shared_ptr<ExpressionNode>& leftOperand,
        const std::shared_ptr<ExpressionNode>& rightOperand,
        const std::shared_ptr<ExpressionNode>& leftDerivative,
        const std::shared_ptr<ExpressionNode>& rightDerivative);

    std::shared_ptr<ExpressionNode> differentiateDivision(
        const std::shared_ptr<ExpressionNode>& leftOperand,
        const std::shared_ptr<ExpressionNode>& rightOperand,
        const std::shared_ptr<ExpressionNode>& leftDerivative,
        const std::shared_ptr<ExpressionNode>& rightDerivative);
    
    std::shared_ptr<ExpressionNode> differentiateExponentiation(
        const std::shared_ptr<ExpressionNode>& base,
        const std::shared_ptr<ExpressionNode>& exponent);

    std::shared_ptr<ExpressionNode> differentiateUnaryOperation(
        const std::string& op,
        const std::shared_ptr<ExpressionNode>& operand,
        const std::shared_ptr<ExpressionNode>& operandDerivative);


};
