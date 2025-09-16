#pragma once

#include <memory>
#include "visitor.h"

#include "../nodes/constant_node.h"
#include "../nodes/variable_node.h"
#include "../nodes/operator_node.h"

class SimplifyVisitor : public Visitor {
public:
	std::shared_ptr<ExpressionNode> result;
	SimplifyVisitor();
	void visit(ConstantNode& node) override;
	void visit(VariableNode& node) override;
	void visit(OperatorNode& node) override;


private:
    std::shared_ptr<ConstantNode> createConstant(
        const double& value);    
    
    bool areEquivalent(const std::shared_ptr<ExpressionNode>& left,
        const std::shared_ptr<ExpressionNode>& right);

    std::shared_ptr<ExpressionNode> simplifyAddition(
        std::shared_ptr<ExpressionNode> left,
        std::shared_ptr<ExpressionNode> right);

    std::shared_ptr<ExpressionNode> simplifySubtraction(
        std::shared_ptr<ExpressionNode> left,
        std::shared_ptr<ExpressionNode> right);

    std::shared_ptr<ExpressionNode> simplifyMultiplication(
        std::shared_ptr<ExpressionNode> left,
        std::shared_ptr<ExpressionNode> right);

    std::shared_ptr<ExpressionNode> simplifyDivision(
        std::shared_ptr<ExpressionNode> left,
        std::shared_ptr<ExpressionNode> right);

    std::shared_ptr<ExpressionNode> simplifyExponentiation(
        std::shared_ptr<ExpressionNode> base,
        std::shared_ptr<ExpressionNode> exponent);    
    
    std::shared_ptr<ExpressionNode> simplifyUnaryOperation(
        const std::string& op,
        std::shared_ptr<ExpressionNode> operand);
};