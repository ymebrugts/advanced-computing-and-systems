/**
 * author Yme Brugts (s4536622)
 * @file program_runner.cpp
 * @date 2025-09-16
 */


#include "program_runner.h"
#include "moviesearch.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

    const std::string HELP_MESSAGE =
        "Available commands:\n"
        "  moviesearch [options]    Prepare a movie search query (parsing only)\n"
        "    --title <keywords>     Title keywords (multi-word allowed; AND semantics)\n"
        "    --year  <YYYY>         Exact release year\n"
        "    --genre <g1,g2,...>    One or more genres (comma-separated or repeat flag)\n"
        "    --tag   <t1,t2,...>    One or more tags (comma-separated or repeat flag)\n"
        "\n"
        "Notes:\n"
        "You can also use --option=value.\n"
        "\n"
        "Examples:\n"
        "  moviesearch --title Blood --tag Upton\n"
        "  moviesearch --title \"Las Vegas\"\n";
	

    void RunProgram(std::istream& in, std::ostream& out, bool interactiveMode) {
        if (interactiveMode) {
            out << HELP_MESSAGE << std::endl;
        }

        std::string line;
        while (true) {
            if (interactiveMode) {
                out << "Enter command: ";
                out.flush();
            }

            if (!std::getline(in, line)) break;
            if (line.empty()) continue;

            // Tokenize respecting quotes
            auto tokens = moviesearch::tokenize_command_line(line);
            if (tokens.empty()) continue;

            const std::string& cmd = tokens[0];

            if (cmd == "moviesearch") {
                auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
                auto res = moviesearch::parse_moviesearch_tokens(args);
                for (const auto& w : res.warnings) out << "Warning: " << w << "\n";
                if (!res.ok) {
                    for (const auto& e : res.errors) out << "Error: " << e << "\n";
                    continue;
                }
                moviesearch::print_query(out, res.query);
            }
            else if (cmd == "help") {
                out << HELP_MESSAGE << std::endl;
            }
            else if (cmd == "end") {
                break;
            }
            else {
                out << "Error: Unknown command '" << cmd << "'.\n";
            }
        }
    }
}
