/**
* author Yme Brugts (s4536622)
 * @file main.cpp
 * @date 2025-09-16
 */

#include <iostream>
#include "program_runner.h"

void show_help(std::ostream& out) {
    out << "Usage: moviesearch [options]\n"
        << "Options:\n"
        << "  -h, --help                  Show this help message and exit\n"
        << "  -n, --no-menu -d --debug    Run in non-interactive mode\n"
        << "Available commands can be displayed with the help command during runtime"
        << "\n";
}

/**
 * The entry point of the application.
 *
 * This function processes command-line arguments to determine the mode of operation.
 * It can operate in interactive mode, or non-interactive mode based on the arguments
 * provided by the user. The help message is displayed if the help flag is detected.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return An integer indicating the exit status of the program. Returns 0 for successful
 *         execution, or when the help message is shown, and returns other values to
 *         signal different kinds of exits if implemented.
 */
int main(int argc, char** argv) {
#ifdef _WIN32
    std::locale::global(std::locale("en_US.UTF-8"));
    std::cout.imbue(std::locale());
    std::cin.imbue(std::locale());
#endif


    bool interactive_mode = true; // Default to interactive mode

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--no-menu" || arg == "-n" || arg == "--debug" || arg == "-d") {
            interactive_mode = false;
        }
        else if (arg == "--help" || arg == "-h") {
            show_help(std::cout);
            return 0;
        }
    }

    RunProgram(std::cin, std::cout, interactive_mode);
    return 0;
}
