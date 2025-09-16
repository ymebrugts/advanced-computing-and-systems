/**
 * author Yme Brugts (s4536622)
 * @file program_runner.cpp
 * @date 2024-10-09
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <fstream>
#include <memory>

#include "program_runner.h"
#include "nodes/expression_node.h"
#include "nodes/variable_node.h"
#include "nodes/operator_node.h"
#include "nodes/constant_node.h"
#include "parser/tokenizer.h"
#include "parser/parser.h"
#include "utils/string_utils.h"
#include "visitors/simplify_visitor.h"
#include "visitors/eval_visitor.h"
#include "visitors/diff_visitor.h"

const std::string HELP_MESSAGE =
"Available commands:\n"
"  exp <expression>  Set the expression to work with\n"
"  dot <filename>    Generate DOT file of the expression tree\n"
"  print             Print the current expression in infix notation\n"
"  eval <value>      Evaluate the expression with x = <value>\n"
"  simp              Simplify the current expression\n"
"  diff              Differentiate the expression with respect to x\n"
"  help              Show this help message\n"
"  end               Exit the program\n";

/**
 * @function RunProgram
 * @abstract Parses prefix expressions from an input stream, executes commands to print infix notation, generates DOT files for Graphviz visualization, 
   parses user input, simplifies expressions, differentiates expressions and can evaluate expressions.
 * @param 'in' The input stream (`std::istream&`) from which commands and expressions are read.
 * @param 'out' The output stream (`std::ostream&`) to which results, error messages, and notifications are written.
 * @return None.
 * @pre - The input stream `in` must be open and ready for reading.
 *      - The output stream `out` must be open and ready for writing.
 * @post - Processes all input commands until an "end" command is received or the input stream is exhausted.
 *       - The output stream `out` will contain the results of command executions. outfile (a local file) will contain the dot notation
 **/
void RunProgram(std::istream& in, std::ostream& out, bool interactiveMode) {
    std::shared_ptr<ExpressionNode> root = nullptr;
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
            if (expr.empty()) {
                out << "Error: No expression provided after 'exp' command." << std::endl;
                continue;
            }
            std::queue<std::string> tokens;
            try {
                tokens = tokenize(expr);
            }
            catch (const std::exception& e) {
                out << "Error tokenizing: " << e.what() << std::endl;
                return;
            }
            try {
                root = parseExpression(tokens);
                if (!tokens.empty()) {
                    out << "Warning: Extra tokens remaining after parsing." << std::endl;
                }
            }
            catch (const std::exception& e) {
                out << "Error parsing expression: " << e.what() << std::endl;
                root = nullptr;
            }
        }
        else if (command == "dot") {
            std::string filename;
            iss >> filename;
            if (filename.empty()) {
                out << "Error: dot command requires a filename." << std::endl;
            }
            else if (!root) {
                out << "Error: No expression to output." << std::endl;
            }
            else {
                std::ofstream outfile(filename);
                if (!outfile) {
                    out << "Error: Cannot open file " << filename << " for writing." << std::endl;
                }
                else {
                    outfile << "digraph G {" << std::endl;
                    int nodeId = 1;
                    root->writeDot(outfile, nodeId);
                    outfile << "}";
                }
            }
        }
        else if (command == "print") {
            if (!root) {
                out << "Error: No expression to print." << std::endl;
            }
            else {
                root->printInfix(out);
                out << std::endl;
            }
        }
        else if (command == "eval") {
            std::string evalValue;
            iss >> evalValue;
            if (evalValue.empty()) {
                out << "Error: No value for X provided after 'eval' command." << std::endl;
                continue;
            }
            if (!is_number(evalValue)) {
                out << "Error: Value for X must be a number." << std::endl;
            }

            if (!root) {
                out << "Error: No expression to evaluate." << std::endl;
            }
            else {
                EvalVisitor evalVisitor("x", evalValue);
                root->accept(evalVisitor);
                root = evalVisitor.result;

                SimplifyVisitor simplifyVisitor;
                root->accept(simplifyVisitor);
                root = simplifyVisitor.result;
            }

        }
        else if (command == "simp") {
            if (!root) {
                out << "Error: No expression to simplify." << std::endl;
                return;
            }

            SimplifyVisitor simplifyVisitor;
            root->accept(simplifyVisitor);
            root = simplifyVisitor.result; // Update root to the simplified expression
            
        }
        else if (command == "diff") {
            if (!root) {
                out << "Error: No expression to simplify." << std::endl;
                return;
            }

            DiffVisitor diffVisitor("x");
            root->accept(diffVisitor);
            root = diffVisitor.result;

        }
        else if (command == "help") {
            out << HELP_MESSAGE << std::endl;
        }
        else if (command == "end") {
            break;
        }
        else {
            out << "Error: No command found." << std::endl;
        }
    }
}

