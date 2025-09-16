
# Prefix Expression Parser
Author<br>
**Yme Brugts** (s4536622)

## Overview

The **Prefix Expression Parser** is a C++ program that parses prefix (Polish notation) mathematical expressions, converts to them infix notation, differentiates, simplifies, evaluates and generates DOT files for visualization with Graphviz.


## Features

- **Tokenization:** Splits input strings into tokens based on whitespace.
- **Parsing:** Constructs an expression tree from prefix tokens.
- **Infix Printing:** Outputs expressions in human-readable infix format.
- **DOT Generation:** Creates DOT notation files for graphical visualization.
- **Simplification:** Simplifies the expression as much as possible.
- **Evaluation:** Evaluates the expression.
- **Differentiation:** Differentiates the expression with respect to x.
- **Command Handling:** Supports `exp`, `print`, `dot`, `simp`, `eval`, `diff`, `help` and `end` commands.


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

- **`exp <expression>`:** Parse a prefix expression to set the current expression.
  - *Example:* `exp + 3 * 4 5`
- **`print`:** Display the last parsed expression in infix notation.
- **`dot <filename>`:** Generate a DOT file for the last parsed expression. This can be used to visualize the expression tree using Graphviz or similar tools.
  - *Example:* `dot expression.dot`
- **`eval <value>`:** Evaluate the current expression by substituting `x` with `<value>`.
  - *Example:* `eval 2`
- **`simp`:** Simplify the current expression.
- **`diff`:** Differentiate the current expression with respect to `x`.
- **`help`:** Show the list of available commands and their descriptions.
- **`end`:** Exit the program.


```
exp + 3 * 4 5
print
dot expression.dot
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



## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
