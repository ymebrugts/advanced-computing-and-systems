#pragma once

#include <memory>
#include "visitor.h"

#include "../nodes/constant_node.h"
#include "../nodes/variable_node.h"
#include "../nodes/operator_node.h"

class EvalVisitor : public Visitor {
public:
    std::shared_ptr<ExpressionNode> result;
    std::string variableName;
    std::string variableValue;

    EvalVisitor(const std::string& varName, const std::string& varValue);

    void visit(ConstantNode& node) override;
    void visit(VariableNode& node) override;
    void visit(OperatorNode& node) override;
};
