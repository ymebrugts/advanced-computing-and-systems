#pragma once
/**
* author Yme Brugts (s4536622)
 * @file matcher_service.h
 * @date 2024-11-30
 */

#include "../models/automaton.h"
#include <string>
#include <vector>

bool matchString(const std::vector<AutomatonState>& automaton, const std::string& input);

