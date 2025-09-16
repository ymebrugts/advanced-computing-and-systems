#pragma once
/**
* author Yme Brugts (s4536622)
 * @file auatomaton.h
 * @date 2024-11-18
 */

#include "../models/automaton.h"
#include <string>
#include <vector>

void exportToDot(const std::vector<AutomatonState>& automaton, const std::string& filename);
