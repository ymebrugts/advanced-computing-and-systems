#include "pch.h"
#include <regex>

#include "../DsPatternRecognition/src/program_runner.h"
#include "../DsPatternRecognition/src/program_runner.cpp"
#include "../DsPatternRecognition/src/parsing/lexer.h"
#include "../DsPatternRecognition/src/parsing/lexer.cpp"
#include "../DsPatternRecognition/src/parsing/parser.h"
#include "../DsPatternRecognition/src/parsing/parser.cpp"
#include "../DsPatternRecognition/src/models/automaton.h"
#include "../DsPatternRecognition/src/services/dot_exporter_service.h"
#include "../DsPatternRecognition/src/services/dot_exporter_service.cpp"
#include "../DsPatternRecognition/src/services/matcher_service.h"
#include "../DsPatternRecognition/src/services/matcher_service.cpp"
#include "../DsPatternRecognition/src/utils/string_trim.cpp"
#include "../DsPatternRecognition/src/utils/string_trim.h"
#include "../DsPatternRecognition/src/utils/sort_by_member.h"
#include "../DsPatternRecognition/src/utils/find_by_member.h"



const std::string TEST_FILE_NAME = "testfile.dot";


std::string CaptureOutput(const std::string& input) {
    std::istringstream inStream(input);
    std::ostringstream outStream;

    RunProgram(inStream, outStream, false);

    std::string output = outStream.str();
    size_t end = output.find_last_not_of(" \n\r\t");
    if (end != std::string::npos) {
        output = output.substr(0, end + 1);
    }
    else {
        output = "";
    }
    return output;
}

std::string sortOutput(const std::string& output) {
    std::vector<std::string> lines;
    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    std::sort(lines.begin(), lines.end());
    std::ostringstream oss;
    for (const auto& l : lines) {
        oss << l << "\n";
    }
    return oss.str();
}



// Function to normalize GraphViz output by sorting nodes and edges
std::string NormalizeGraphVizOutput(const std::string& dotOutput) {
    std::vector<std::string> globalAttributes;
    std::vector<std::string> nodeAttributes;
    std::vector<std::string> nodes;
    std::vector<std::string> edges;

    std::istringstream stream(dotOutput);
    std::string line;

    // Regular expressions for global attributes, node attribute settings, nodes, and edges
    std::regex globalRegex(R"(^\s*(rankdir|size)\s*=.*;)");
    std::regex nodeAttrRegex(R"(^\s*node\s*\[.*\]\s*;)");
    std::regex nodeRegex(R"(^\s*\w+\s*\[.*\]\s*;)");
    std::regex edgeRegex(R"(^\s*\w+\s*->\s*\w+(\s*\[.*\])?\s*;)");

    // Process each line and categorize it
    while (std::getline(stream, line)) {
        if (std::regex_match(line, globalRegex)) {
            globalAttributes.push_back(line);
        }
        else if (std::regex_match(line, nodeAttrRegex)) {
            nodeAttributes.push_back(line);
        }
        else if (std::regex_match(line, nodeRegex)) {
            nodes.push_back(line);
        }
        else if (std::regex_match(line, edgeRegex)) {
            edges.push_back(line);
        }
        // Optionally handle comments or other elements here
    }

    // Sort nodes and edges
    std::sort(nodes.begin(), nodes.end());
    std::sort(edges.begin(), edges.end());

    // Reconstruct the normalized output
    std::ostringstream normalizedOutput;
    normalizedOutput << "digraph AutomatonNormalized {\n";

    // Add global attributes
    for (const auto& attr : globalAttributes) {
        normalizedOutput << "  " << attr << "\n";
    }

    // Add node attribute settings
    for (const auto& nodeAttr : nodeAttributes) {
        normalizedOutput << "  " << nodeAttr << "\n";
    }

    // Add sorted nodes
    for (const auto& node : nodes) {
        normalizedOutput << "  " << node << "\n";
    }

    // Add sorted edges
    for (const auto& edge : edges) {
        normalizedOutput << "  " << edge << "\n";
    }

    normalizedOutput << "}\n";

    return normalizedOutput.str();
}


// Function to capture the GraphViz output by writing to a temporary file
std::string CaptureGraphVizOutput(const std::string& input) {
    std::istringstream inStream(input);
    std::ostringstream outStream;
    RunProgram(inStream, outStream, false);

    std::ifstream infile(TEST_FILE_NAME);
    std::string output;
    if (infile) {
        std::ostringstream fileStream;
        fileStream << infile.rdbuf();
        output = fileStream.str();
        infile.close();
    }

    std::remove(TEST_FILE_NAME.c_str());

    return output;
}

TEST(RegexParserTest, ValidExpression1) {
    std::string expr = "a(b|c)*";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}

TEST(RegexParserTest, ValidExpression2) {
    std::string expr = "(a|b)c*d";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}

TEST(RegexParserTest, ValidExpression4) {
    std::string expr = "abc";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}

TEST(RegexParserTest, ValidExpression5) {
    std::string expr = "a|b|c";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}

TEST(RegexParserTest, ValidExpression6) {
    std::string expr = "a(b(c(d(e)*)*)*)*";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}

TEST(RegexParserTest, ValidExpression7) {
    std::string expr = "a*";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}

TEST(RegexParserTest, ValidExpression8) {
    std::string expr = "a(b|c)d*e*f(g|h)";
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_EQ(output.size(), 0) <<  output << expr;
}


TEST(RegexParserTest, InvalidExpression1) {
    std::string expr = "a(b|c"; // Missing closing parenthesis
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression2) {
    std::string expr = "(a|b))*c"; // Extra closing parenthesis
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression3) {
    std::string expr = "a**"; // Invalid use of '*'
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression4) {
    std::string expr = "|abc"; // Leading '|'
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression5) {
    std::string expr = "abc|"; // Trailing '|'
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression6) {
    std::string expr = "a(b|)"; // Empty alternative
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression7) {
    std::string expr = "a(b c)"; // Missing operator
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error: Expression contains spaces"), std::string::npos) << output << expr;
}

TEST(RegexParserTest, InvalidExpression8) {
    std::string expr = "a(b|c))("; // Mismatched parentheses
    std::string input = "exp " + expr + "\nend\n";
    std::string output = CaptureOutput(input);
    EXPECT_NE(output.find("Error while tokenizing and parsing"), std::string::npos) << output << expr;
}

TEST(AutomatonExportTest, InvalidExpression) {
    std::string expr = "a**";
    std::string input = "exp " + expr + "\ndot " + TEST_FILE_NAME + "\nend\n";

    std::string actualOutput = CaptureGraphVizOutput(input);
    EXPECT_TRUE(actualOutput.empty()) << "Dot output should be empty for invalid expression";
}

TEST(AutomatonExportTest, Alternation) {
    std::string expr = "a|b";
    std::string input = "exp " + expr + "\ndot " + TEST_FILE_NAME + "\nend\n";

    std::string expectedOutput =
        "digraph G {\n"
        "  rankdir=LR;\n"
        "  node [shape = circle];\n"
        "  start [shape=point];\n"
        "  start -> 0;\n"
        "  0 -> 1 [label=\"&epsilon;\"];\n"
        "  0 -> 3 [label=\"&epsilon;\"];\n"
        "  1 -> 2 [label=\"a\"];\n"
        "  3 -> 4 [label=\"b\"];\n"
        "  2 -> 5 [label=\"&epsilon;\"];\n"
        "  4 -> 5 [label=\"&epsilon;\"];\n"
        "  5 [shape=doublecircle];\n"
        "}\n";

    // Normalize the expected and actual output for comparison
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);

    // Act
    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedOutput = NormalizeGraphVizOutput(actualOutput);

    EXPECT_EQ(normalizedOutput, normalizedExpected);
}

TEST(AutomatonPrintTest, PrintAlternation) {
    std::string input = "exp a|b\nprint\nend\n";

    std::string expectedOutput =
        "0, -, 1, 3\n"
        "1, a, 2, -1\n"
        "3, b, 4, -1\n"
        "2, -, 5, -1\n"
        "4, -, 5, -1\n"
        "5, -, -1, -1";

    std::string actualOutput = CaptureOutput(input);
    std::string sortedActualOutput = sortOutput(actualOutput);
    std::string sortedExpectedOutput = sortOutput(expectedOutput);

    EXPECT_EQ(sortedActualOutput, sortedExpectedOutput);
}

TEST(AutomatonPrintTest, PrintKleeneStar) {
    std::string input = "exp a*\nprint\nend\n";

    std::string expectedOutput =
        "0, -, 3, -1\n"
        "1, a, 2, -1\n"
        "2, -, 1, 4\n"
        "3, -, 1, 4\n"
        "4, -, -1, -1";

    // Act
    std::string actualOutput = CaptureOutput(input);

    // Sort the outputs to ignore ordering differences
    std::string sortedActualOutput = sortOutput(actualOutput);
    std::string sortedExpectedOutput = sortOutput(expectedOutput);

    // Assert
    EXPECT_EQ(sortedActualOutput, sortedExpectedOutput);
}

TEST(AutomatonExportTest, KleeneStar) {
    std::string expr = "a*";
    std::string input = "exp " + expr + "\ndot " + TEST_FILE_NAME + "\nend\n";

    std::string expectedOutput =
        "digraph G {\n"
        "  rankdir=LR;\n"
        "  node [shape = circle];\n"
        "  start [shape=point];\n"
        "  start -> 0;\n"
        "  0 -> 3 [label=\"&epsilon;\"];\n"
        "  1 -> 2 [label=\"a\"];\n"
        "  2 -> 1 [label=\"&epsilon;\"];\n"
        "  2 -> 4 [label=\"&epsilon;\"];\n"
        "  3 -> 1 [label=\"&epsilon;\"];\n"
        "  3 -> 4 [label=\"&epsilon;\"];\n"
        "  4 [shape=doublecircle];\n"
        "}\n";

    // Normalize the expected and actual output for comparison
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);

    // Act
    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedOutput = NormalizeGraphVizOutput(actualOutput);

    // Assert
    EXPECT_EQ(normalizedOutput, normalizedExpected);
}

TEST(AutomatonPrintTest, PrintSimpleLetter) {
    std::string input = "exp a\nprint\nend\n";

    std::string expectedOutput =
        "0, -, 1, -1\n"
        "1, a, 2, -1\n"
        "2, -, -1, -1";

    // Act
    std::string actualOutput = CaptureOutput(input);

    // Sort the outputs to ignore ordering differences (if necessary)
    std::string sortedActualOutput = sortOutput(actualOutput);
    std::string sortedExpectedOutput = sortOutput(expectedOutput);

    // Assert
    EXPECT_EQ(sortedActualOutput, sortedExpectedOutput);
}

TEST(AutomatonExportTest, SimpleLetter) {
    std::string expr = "a";
    std::string input = "exp " + expr + "\ndot " + TEST_FILE_NAME + "\nend\n";

    std::string expectedOutput =
        "digraph G {\n"
        "  rankdir=LR;\n"
        "  node [shape = circle];\n"
        "  start [shape=point];\n"
        "  start -> 0;\n"
        "  0 -> 1 [label=\"&epsilon;\"];\n"
        "  1 -> 2 [label=\"a\"];\n"
        "  2 [shape=doublecircle];\n"
        "}\n";

    // Normalize the expected and actual output for comparison
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);

    // Act
    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedOutput = NormalizeGraphVizOutput(actualOutput);

    // Assert
    EXPECT_EQ(normalizedOutput, normalizedExpected);
}

TEST(AutomatonPrintTest, PrintConcatenation) {
    std::string input = "exp ab\nprint\nend\n";

    std::string expectedOutput =
        "0, -, 1, -1\n"
        "1, a, 2, -1\n"
        "2, -, 3, -1\n"
        "3, b, 4, -1\n"
        "4, -, -1, -1";

    // Act
    std::string actualOutput = CaptureOutput(input);

    // Sort the outputs to ignore ordering differences (if necessary)
    std::string sortedActualOutput = sortOutput(actualOutput);
    std::string sortedExpectedOutput = sortOutput(expectedOutput);

    // Assert
    EXPECT_EQ(sortedActualOutput, sortedExpectedOutput);
}

TEST(AutomatonExportTest, ConcatenationExpression) {
    std::string expr = "ab";
    std::string input = "exp " + expr + "\ndot " + TEST_FILE_NAME + "\nend\n";

    std::string expectedOutput =
        "digraph G {\n"
        "  rankdir=LR;\n"
        "  node [shape = circle];\n"
        "  start [shape=point];\n"
        "  start -> 0;\n"
        "  0 -> 1 [label=\"&epsilon;\"];\n"
        "  1 -> 2 [label=\"a\"];\n"
        "  2 -> 3 [label=\"&epsilon;\"];\n"
        "  3 -> 4 [label=\"b\"];\n"
        "  4 [shape=doublecircle];\n"
        "}\n";

    // Normalize the expected and actual output for comparison
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);

    // Act
    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedOutput = NormalizeGraphVizOutput(actualOutput);

    // Assert
    EXPECT_EQ(normalizedOutput, normalizedExpected);
}


// Automaton for "a"
std::vector<AutomatonState> getAutomatonA() {
    return {
        {0, '-', 1, -1},
        {1, 'a', 2, -1},
        {2, '-', -1, -1}
    };
}

// Automaton for "a|b"
std::vector<AutomatonState> getAutomatonAorB() {
    return {
        {0, '-', 1, 3},
        {1, 'a', 2, -1},
        {3, 'b', 4, -1},
        {2, '-', 5, -1},
        {4, '-', 5, -1},
        {5, '-', -1, -1}
    };
}

// Automaton for "ab"
std::vector<AutomatonState> getAutomatonAB() {
    return {
        {0, '-', 1, -1},
        {1, 'a', 2, -1},
        {2, '-', 3, -1},
        {3, 'b', 4, -1},
        {4, '-', -1, -1}
    };
}

// Automaton for "a*"
std::vector<AutomatonState> getAutomatonAStar() {
    return {
        {0, '-', 1, -1},
        {1, '-', 2, 4},
        {2, 'a', 3, -1},
        {3, '-', 1, 4},
        {4, '-', -1, -1}
    };
}

// Automaton for "(a|b)*c"
std::vector<AutomatonState> getAutomatonAorBStarC() {
    return {
        {0, '-', 1, -1},
        {1, '-', 2, 4},
        {2, '-', 3, -1},
        {3, 'a', 5, -1},
        {4, '-', 6, -1},
        {6, 'b', 7, -1},
        {5, '-', 1, -1},
        {7, '-', 1, -1},
        {1, '-', 8, -1},
        {8, 'c', 9, -1},
        {9, '-', -1, -1}
    };
}

// Test cases for "a"
TEST(MatcherTest, MatchA) {
    std::vector<AutomatonState>& automaton = getAutomatonA();

    EXPECT_TRUE(matchString(automaton, "a"));
    EXPECT_FALSE(matchString(automaton, ""));
    EXPECT_FALSE(matchString(automaton, "b"));
    EXPECT_FALSE(matchString(automaton, "aa"));
    EXPECT_FALSE(matchString(automaton, "ab"));
}

// Test cases for "a|b"
TEST(MatcherTest, MatchAorB) {
    std::vector<AutomatonState>& automaton = getAutomatonAorB();

    EXPECT_TRUE(matchString(automaton, "a"));
    EXPECT_TRUE(matchString(automaton, "b"));
    EXPECT_FALSE(matchString(automaton, ""));
    EXPECT_FALSE(matchString(automaton, "ab"));
    EXPECT_FALSE(matchString(automaton, "ba"));
    EXPECT_FALSE(matchString(automaton, "c"));
}

// Test cases for "ab"
TEST(MatcherTest, MatchAB) {
    std::vector<AutomatonState>& automaton = getAutomatonAB();

    EXPECT_TRUE(matchString(automaton, "ab"));
    EXPECT_FALSE(matchString(automaton, "a"));
    EXPECT_FALSE(matchString(automaton, "b"));
    EXPECT_FALSE(matchString(automaton, "aa"));
    EXPECT_FALSE(matchString(automaton, "ba"));
    EXPECT_FALSE(matchString(automaton, "abc"));
}

// Test cases for "a*"
TEST(MatcherTest, MatchAStar) {
    std::vector<AutomatonState>& automaton = getAutomatonAStar();

    EXPECT_TRUE(matchString(automaton, ""));
    EXPECT_TRUE(matchString(automaton, "a"));
    EXPECT_TRUE(matchString(automaton, "aa"));
    EXPECT_TRUE(matchString(automaton, "aaa"));
    EXPECT_FALSE(matchString(automaton, "b"));
    EXPECT_FALSE(matchString(automaton, "ab"));
}

// Test cases for "(a|b)*c"
TEST(MatcherTest, MatchAorBStarC) {
    std::vector<AutomatonState>& automaton = getAutomatonAorBStarC();

    EXPECT_TRUE(matchString(automaton, "c"));
    EXPECT_TRUE(matchString(automaton, "ac"));
    EXPECT_TRUE(matchString(automaton, "bc"));
    EXPECT_TRUE(matchString(automaton, "aabc"));
    EXPECT_TRUE(matchString(automaton, "abababc"));
    EXPECT_FALSE(matchString(automaton, ""));
    EXPECT_FALSE(matchString(automaton, "a"));
    EXPECT_FALSE(matchString(automaton, "ab"));
    EXPECT_TRUE(matchString(automaton, "abc"));
    EXPECT_FALSE(matchString(automaton, "caa"));
}


