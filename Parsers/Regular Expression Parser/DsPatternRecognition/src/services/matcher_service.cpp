/**
* author Yme Brugts (s4536622)
 * @file matcher_service.cpp
 * @date 2024-11-30
 */

#include "matcher_service.h"
#include <set>
#include "../utils/sort_by_member.h"

/**
 * Computes the epsilon closure for a set of states in a nondeterministic finite automaton.
 *
 * The epsilon closure is the set of states that can be reached from any of the given states
 * by traversing transitions marked with the epsilon symbol ('-'). This function will iterate
 * through the automaton and discover all such states, augmenting the input set of states.
 *
 * @param automaton The vector representing the states and transitions of the automaton.
 *                  Each state includes information about its source node, input symbol, and branches.
 * @param states A set of state indices representing the initial states. This set will be updated
 *               to include all states reachable through epsilon transitions from the initial set.
 */
void epsilonClosure(const std::vector<AutomatonState>& automaton, std::set<int>& states) {
    std::set<int> stack(states);
    while (!stack.empty()) {
        int state = *stack.begin();
        stack.erase(stack.begin());

        for (const auto& trans : automaton) {
            if (trans.sourceNode == state && trans.inputSymbol == '-') {
                if (trans.branch1 != -1 && states.insert(trans.branch1).second) {
                    stack.insert(trans.branch1);
                }
                if (trans.branch2 != -1 && states.insert(trans.branch2).second) {
                    stack.insert(trans.branch2);
                }
            }
        }
    }
}

/**
 * Determines if a given input string can be matched by a specified nondeterministic finite automaton (NFA).
 *
 * This function implements state transition processing for the provided automaton against the input
 * string, utilizing epsilon transitions to explore all possible paths through the automaton. The
 * automaton transitions must be pre-sorted by source node for the function to operate correctly.
 *
 * @param unsortedAutomaton A vector of AutomatonState objects representing the NFA, containing
 *                          information about each state's source node, input symbol, and branching
 *                          transitions.
 * @param input The string input that will be evaluated against the NFA to determine if it
 *              can reach an accept state.
 * @return Returns true if the input string can be matched by the automaton (i.e., reaches an
 *         accept state), otherwise returns false.
 */
bool matchString(const std::vector<AutomatonState>& unsortedAutomaton, const std::string& input) {
    if (unsortedAutomaton.empty()) {
        return false;
    }
    auto automaton = unsortedAutomaton;
    sortByMember(automaton, &AutomatonState::sourceNode, true);

    std::set<int> currentStates;
    currentStates.insert(automaton.front().sourceNode);
    epsilonClosure(automaton, currentStates);

    // Check for accept state after epsilon closure
    if (input.empty()) {
        for (int state : currentStates) {
            for (const auto& trans : automaton) {
                if (trans.sourceNode == state && trans.inputSymbol == '-' && trans.branch1 == -1 && trans.branch2 == -1) {
                    return true; // Accept the empty string
                }
            }
        }
    }


    for (char c : input) {
        std::set<int> nextStates;

        for (int state : currentStates) {
            for (const auto& trans : automaton) {
                if (trans.sourceNode == state && trans.inputSymbol == c) {
                    if (trans.branch1 != -1) {
                        nextStates.insert(trans.branch1);
                    }
                    if (trans.branch2 != -1) {
                        nextStates.insert(trans.branch2);
                    }
                }
            }
        }

        // Apply epsilon closure to nextStates
        epsilonClosure(automaton, nextStates);

        currentStates = nextStates;
        if (currentStates.empty()) {
            return false;
        }
    }

    // Check if any of the currentStates is an accept state
    for (int state : currentStates) {
        for (const auto& trans : automaton) {
            if (trans.sourceNode == state && trans.inputSymbol == '-' && trans.branch1 == -1 && trans.branch2 == -1) {
                return true;
            }
        }
    }

    return false;
}
