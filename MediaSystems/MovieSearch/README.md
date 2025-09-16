
# Prefix Expression Parser
Author<br>
**Yme Brugts** (s4536622)

## Overview

The **Regular Expression parser** is a C++ program that can parse regular expressions, create a Finite automaton from it and export them to the Graphiz DOT notation for visualization.

## Features

- **Tokenization:** Splits input strings into tokens.
- **Parsing:** Constructs a non-deterministic Finite Automaton (NFA) representation from a regular expression
- **DOT Generation:** Outputs the NFA representation to Graphiz dot format for graphical visualization.
- **Command Handling:** Supports `exp`, `print`, `dot`, `help` and `end` commands.


## Installation

### Prerequisites

- **C++ Compiler:** `g++` or `clang++`
- **Make:** Unix-compatible `make`
- **Google Test:** For running unit tests


### Build Instructions
1. **Clean Build Artifacts:**
   ```bash
   make clean
   ```

2. **Compile the Program:**
   ```bash
   make
   ```


## Usage
```bash
./main < input.txt
```
or just:
```bash
./main
```

### Options:
- `-h`, `--help`  
  Show this help message and exit.

- `-n`, `--no-menu`, `-d`, `--debug`   
  Run in non-interactive mode.

```bash
./main --no-menu < input.txt
```
or just:
```bash
./main --no-menu
```

You can change the precision by changing `PRECISIONOUTPUT` in constant_node.cpp to something else

### Supported Commands

- **`exp <expression>`:** Parse a regular expression to an NFA.
  - *Example:* `exp a(a|b)*c`
- **`print`:** Prints the NFA in list form "sourceNode, input symbol, left branch, right branch"
- **`dot <filename>`:** Generate a DOT file for the last parsed regular expression. This can be used to visualize the NFA using Graphviz or similar tools.
  - *Example:* `dot nfa.dot`
- **`help`:** Show the list of available commands and their descriptions.
- **`end`:** Exit the program.


```
exp a(a|b)*c
dot nfa.dot
end
```

## Running Tests

The project uses **Google Test (GTest)** for unit testing. You can run the tests through Visual Studio's Test Explorer.

### Prerequisites

- **Visual Studio** with C++ development tools.
- **Google Test:** If not already set up, follow [these instructions](https://learn.microsoft.com/en-us/visualstudio/test/how-to-use-google-test-for-cpp?view=vs-2022) to configure Google Test with Visual Studio.

### Running Tests in Visual Studio

1. **Open the Project** in Visual Studio.
2. **Build the Tests:** Ensure that your test project is built successfully.
3. **Run Tests:** Open the **Test Explorer** from `Test > Windows > Test Explorer` and run the tests from there.

4. **View Test Results:** The results will be displayed in the Test Explorer. Make sure that all tests pass before proceeding with further development.

### Troubleshooting
- If you get an error when running the tests you can try removing the `packages` folder in the root of this project. It can conflict with the Visual Studio version

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
