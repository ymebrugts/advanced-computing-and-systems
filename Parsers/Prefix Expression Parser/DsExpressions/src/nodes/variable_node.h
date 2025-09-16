#pragma once
#include "expression_node.h"
#include <string>
#include <ostream>

class VariableNode : public ExpressionNode {
public:
    std::string identifier;

    VariableNode(const std::string& identifier);

    bool isOperator() const override;
    int getNodePrecedence() const override;
    void printInfix(std::ostream& out, int parentPrecedence = 0) override;
    int writeDot(std::ostream& out, int& nodeId) override;
    void accept(Visitor& visitor) override;
};