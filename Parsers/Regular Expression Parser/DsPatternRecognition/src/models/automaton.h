#pragma once
/**
* author Yme Brugts (s4536622)
 * @file automaton.h
 * @date 2024-11-18
 */

#include <sstream>
#include <string>

struct AutomatonState {
    int sourceNode;     // State index
    char inputSymbol;   // Input symbol or '-' for lambda-transition
    int branch1;        // First branch (state index or -1)
    int branch2;        // Second branch (state index or -1)


    std::string toString() const {
        std::ostringstream oss;
        oss << sourceNode << ", " << inputSymbol << ", " << branch1 << ", " << branch2;
        return oss.str();
    }
};

struct Fragment {
    int startState;     // Start state of the fragment
    int acceptState;    // Accept state of the fragment
};