/**
* author Yme Brugts (s4536622)
 * @file program_runner.cpp
 * @date 2025-09-16
 */


#include "program_runner.h"
#include "parsing/lexer.h"
#include "parsing/parser.h"
#include "services/dot_exporter_service.h"
#include <sstream>
#include <string>
#include <algorithm>
#include "services/matcher_service.h"
#include "utils/string_trim.h"

const std::string HELP_MESSAGE =
"Available commands:\n"
"  exp <expression>  Sets the regular expression to work with\n"
"  print             Prints the NFA in list form \"sourceNode, input symbol, left branch, right branch\"\n"
"  dot <filename>    Generates a DOT file from a constructed NFA\n"
"  mat               Checks if the automaton will be accepted by the NFA. Input '-' is not allowed. input '$' can be used to denote empty string \n"
"  help              Show this help message\n"
"  end               Exit the program\n";

/**
 * @function RunProgram
 * @abstract Tokenizes, Parses and constructs a FNA from regular expressions in the input stream, executes commands to generate DOT files for Graphviz visualization and
   determines if a string if accepted by the automaton,
 * @param 'in' The input stream (`std::istream&`) from which commands and expressions are read.
 * @param 'out' The output stream (`std::ostream&`) to which results, error messages, and notifications are written.
 * @return None.
 * @pre - The input stream `in` must be open and ready for reading.
 *      - The output stream `out` must be open and ready for writing.
 * @post - Processes all input commands until an "end" command is received or the input stream is exhausted.
 *       - The output stream `out` will contain the results of command executions. outfile (a local file) will contain the dot notation
 **/
void RunProgram(std::istream& in, std::ostream& out, bool interactiveMode) {
    Parser* parser = nullptr;
    std::string line;

    if (interactiveMode) {
        out << HELP_MESSAGE << std::endl;
    }

    while (true) {
        if (interactiveMode) {
            out << "Enter command: ";
            out.flush();
        }

        if (!std::getline(in, line)) {
            break;
        }

        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "exp") {
            std::string expr;
            std::getline(iss, expr);

            // Remove leading and trailing spaces
            expr = string_trim(expr);

            if (expr.empty()) {
                out << "Error: No expression provided after 'exp' command." << std::endl;
                continue;
            }

            // Check if there's a space in the expression
            if (expr.find(' ') != std::string::npos) {
                out << "Error: Expression contains spaces: '" << expr << "'" << std::endl;
                continue;
            }


            // Tokenize and parse
            try {
                std::vector<Token> tokens = tokenize(expr);
                delete parser; // Delete previous parser if any
                parser = new Parser(tokens);
                parser->parse();
            }
            catch (const std::exception& e) {
                out << "Error while tokenizing and parsing: " << e.what() << std::endl;
                delete parser;
                parser = nullptr;
            }
        }
        else if (command == "print") {
            if (!parser) {
                out << "Error: No automaton to print. Parse an expression first.\n";
                continue;
            }
            parser->printAutomaton(out);
        }
        else if (command == "dot") {
            if (!parser) {
                out << "Error: No automaton available. Use 'exp' to define one." << std::endl;
                continue;
            }
            std::string filename;
            iss >> filename;
            if (filename.empty()) {
                out << "Error: dot command requires a filename." << std::endl;
                continue;
            }
            try {
                exportToDot(parser->getAutomaton(), filename);
            }
            catch (const std::exception& e) {
                out << "Error while exporting to Dot format: " << e.what() << std::endl;
            }
        }
        else if (command == "mat") {
            if (!parser) {
                out << "Error: No automaton available. Use 'exp' to define one." << std::endl;
                continue;
            }
            std::string inputStr;
            std::getline(iss, inputStr);

            inputStr = string_trim(inputStr);

            if (inputStr.empty()) {
                out << "Error: No string provided after 'mat' command." << std::endl;
                continue;
            }

            // For empty strings or any empty strings in between input characters
            inputStr.erase(std::remove(inputStr.begin(), inputStr.end(), '$'), inputStr.end());

            if (inputStr.find_first_not_of('-') == std::string::npos) {
                out << "Error: A dash is not allowed as input" << std::endl;
                continue;
            }

            if (matchString(parser->getAutomaton(), inputStr)) {
                out << "match" << std::endl;
            }
            else {
                // Weird to use Dutch here but the autograder might become confused otherwise
                out << "geen match" << std::endl;
            }
        }
        else if (command == "help") {
            out << HELP_MESSAGE << std::endl;
        }
        else if (command == "end") {
            break;
        }
        else {
            out << "Error: Unknown command '" << command << "'." << std::endl;
        }
    }

    delete parser;
}
