#pragma once
#include "expression_node.h"

class ConstantNode : public ExpressionNode {
public:
    std::string value;

    ConstantNode(const std::string& value);

    bool isOperator() const override;
    int getNodePrecedence() const override;
    void printInfix(std::ostream& out, int parentPrecedence = 0) override;
    int writeDot(std::ostream& out, int& nodeId) override;
    void accept(Visitor& visitor) override;
    double getValueAsDouble() const;
    bool isZero();
    bool isOne();

private:
    bool isValueZero = false;
    bool isValueOne = false;
};