#include <iostream>
#include "program_runner.h"

void ShowHelp(std::ostream& out) {
    out << "Usage: DS Expressions 2 [options]\n"
        << "Options:\n"
        << "  -h, --help                  Show this help message and exit\n"
        << "  -n, --no-menu -d --debug    Run in non-interactive mode\n"
        << "Available commands can be displayed with the help command during runtime"
        << "\n";
}

int main(int argc, char** argv) {
    bool interactiveMode = true; // Default to interactive mode

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--no-menu" || arg == "-n" || arg == "--debug" || arg == "-d") {
            interactiveMode = false;
        }
        else if (arg == "--help" || arg == "-h") {
            ShowHelp(std::cout);
            return 0;
        }
    }

    RunProgram(std::cin, std::cout, interactiveMode);
    return 0;
}
