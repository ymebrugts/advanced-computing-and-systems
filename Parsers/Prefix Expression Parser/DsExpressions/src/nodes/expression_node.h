#pragma once
#include <ostream>
#include "../visitors/visitor.h"

class ExpressionNode {
public:
    virtual ~ExpressionNode() = default;
    virtual void printInfix(std::ostream& out, int parentPrecedence = 0) = 0;
    virtual int writeDot(std::ostream& out, int& nodeId) = 0;
    virtual bool isOperator() const = 0;
    virtual int getNodePrecedence() const = 0;
    virtual void accept(Visitor& visitor) = 0;
};
