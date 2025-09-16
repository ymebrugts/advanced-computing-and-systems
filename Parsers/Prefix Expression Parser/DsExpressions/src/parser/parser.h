#pragma once
#include <memory>
#include <queue>
#include <string>
#include "../nodes/expression_node.h"

std::shared_ptr<ExpressionNode> parseExpression(std::queue<std::string>& tokens);