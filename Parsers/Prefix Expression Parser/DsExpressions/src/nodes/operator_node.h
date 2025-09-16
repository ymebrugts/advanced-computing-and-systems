#pragma once
#include "expression_node.h"
#include <string>
#include <vector>
#include <memory>

class OperatorNode : public ExpressionNode {
public:
    std::string op;
    std::vector<std::shared_ptr<ExpressionNode>> operands;

    OperatorNode(const std::string& op);

    bool isOperator() const override;
    int getNodePrecedence() const override;
    void printInfix(std::ostream& out, int parentPrecedence = 0) override;
    int writeDot(std::ostream& out, int& nodeId) override;
    void accept(Visitor& visitor) override;
};
