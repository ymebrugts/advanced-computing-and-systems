/**
* author Yme Brugts (s4536622)
 * @file dot_exporter_service.cpp
 * @date 2024-12-01
 */


#include "dot_exporter_service.h"
#include <fstream>
#include <stdexcept>
#include "../utils/find_by_member.h"


/**
 * Exports an automaton to a DOT file for visualization.
 *
 * This function generates a DOT representation of the given automaton
 * and writes it to a specified file.
 *
 * @param automaton A vector of type AutomatonState containing the states of the automaton to be exported.
 *
 * @param filename A string representing the name of the file to which the DOT
 * representation will be written. The function attempts to open this file for writing,
 * and will throw an exception if it fails to open the file.
 *
 * @throws std::runtime_error Throws a runtime error if unable to open the specified
 * file for writing.
 */
void exportToDot(const std::vector<AutomatonState>& automaton, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    outFile << "digraph Automaton {\n";
    outFile << "  rankdir=LR;\n"; // Left to right orientation
    outFile << "  node [shape = circle];\n";

    // Identify accept states
    std::vector<int> acceptStates;
    for (const auto& state : automaton) {
        if (state.inputSymbol == '-' && state.branch1 == -1 && state.branch2 == -1) {
            acceptStates.push_back(state.sourceNode);
        }
    }

    // Mark accept states
    for (int acceptState : acceptStates) {
        outFile << "  " << acceptState << " [shape=doublecircle];\n";
    }

    // Transitions
    for (const auto& state : automaton) {
        std::string label = (state.inputSymbol == '-') ? "&epsilon;" : std::string(1, state.inputSymbol);
        if (state.branch1 != -1) {
            outFile << "  " << state.sourceNode << " -> " << state.branch1 << " [label=\"" << label << "\"];\n";
        }
        if (state.branch2 != -1) {
            outFile << "  " << state.sourceNode << " -> " << state.branch2 << " [label=\"" << label << "\"];\n";
        }
    }

    auto startNode = findByMember(automaton, 0, &AutomatonState::sourceNode);

    // Initial state indicator
    if (!automaton.empty() && startNode != automaton.end()) {
        outFile << "  start [shape=point];\n";
        outFile << "  start -> " << startNode->sourceNode << ";\n";
    }

    outFile << "}\n";
    outFile.close();
}
