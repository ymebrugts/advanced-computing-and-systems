#pragma once

// Forward declarations needed to avoid circular dependency
class ConstantNode;
class VariableNode;
class OperatorNode;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(ConstantNode& node) = 0;
    virtual void visit(VariableNode& node) = 0;
    virtual void visit(OperatorNode& node) = 0;
};
