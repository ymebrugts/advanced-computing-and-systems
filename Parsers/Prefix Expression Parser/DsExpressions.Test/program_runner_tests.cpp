#include "pch.h"

#include "../DsExpressions/src/program_runner.h"
#include "../DsExpressions/src/program_runner.cpp" 
#include "../DsExpressions/src/parser/parser.h"
#include "../DsExpressions/src/parser/parser.cpp"
#include "../DsExpressions/src/parser/tokenizer.cpp"
#include "../DsExpressions/src/parser/tokenizer.h"
                          
#include "../DsExpressions/src/nodes/expression_node.h"
#include "../DsExpressions/src/nodes/variable_node.h"
#include "../DsExpressions/src/nodes/operator_node.h"
#include "../DsExpressions/src/nodes/constant_node.h"
#include "../DsExpressions/src/nodes/variable_node.cpp"
#include "../DsExpressions/src/nodes/operator_node.cpp"
#include "../DsExpressions/src/nodes/constant_node.cpp"

#include "../DsExpressions/src/visitors/simplify_visitor.h"
#include "../DsExpressions/src/visitors/simplify_visitor.cpp"
#include "../DsExpressions/src/visitors/visitor.h"
#include "../DsExpressions/src/visitors/eval_visitor.h"
#include "../DsExpressions/src/visitors/eval_visitor.cpp"
#include "../DsExpressions/src/visitors/diff_visitor.h"
#include "../DsExpressions/src/visitors/diff_visitor.cpp"

#include "../DsExpressions/src/utils/string_utils.h"
#include "../DsExpressions/src/utils/string_utils.cpp"
#include "../DsExpressions/src/utils/double_utils.h"
#include "../DsExpressions/src/utils/double_utils.cpp"

#include <regex>

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

// Test Case: EvaluateExampleExpression
TEST(EvalTests, EvaluateExampleExpression) {
    std::string input = "exp * x cos * * pi x 2\ndiff\neval 2\nprint\nend\n";
    std::string expectedOutput = "1";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithOnlyX
TEST(EvalTests, EvaluateExpressionWithOnlyX) {
    std::string input = "exp + * 2 x 3\neval 4\nprint\nend\n";
    std::string expectedOutput = "11";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithXAndY
TEST(EvalTests, EvaluateExpressionWithXAndY) {
    std::string input = "exp + * x y 5\neval 3\nprint\nend\n";
    std::string expectedOutput = "3*y+5";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithYOnly
TEST(EvalTests, EvaluateExpressionWithYOnly) {
    std::string input = "exp * y 2\neval 4\nprint\nend\n";
    std::string expectedOutput = "y*2";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateComplexExpressionWithXAndOthers
TEST(EvalTests, EvaluateComplexExpressionWithXAndOthers) {
    std::string input = "exp + * x x * y y\neval 2\nprint\nend\n";
    std::string expectedOutput = "4+y*y";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithMultipleX
TEST(EvalTests, EvaluateExpressionWithMultipleX) {
    std::string input = "exp * + x 3 - x 1\neval 5\nprint\nend\n";
    std::string expectedOutput = "32";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionToNumericalResult
TEST(EvalTests, EvaluateExpressionToNumericalResult) {
    std::string input = "exp / * + x 2 4 - x 2\neval 3\nprint\nend\n";
    std::string expectedOutput = "20";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithUndefinedVariable
TEST(EvalTests, EvaluateExpressionWithUndefinedVariable) {
    std::string input = "exp + x z\neval 2\nprint\nend\n";
    std::string expectedOutput = "2+z";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithTrigFunction
TEST(EvalTests, EvaluateExpressionWithTrigFunction) {
    std::string input = "exp sin x\neval 0\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: EvaluateExpressionWithPi
TEST(EvalTests, EvaluateExpressionWithPi) {
    std::string input = "exp * x pi\neval 2\nprint\nend\n";
    std::string expectedOutput = "6.28319";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}


// Test Case: EvaluateExpressionWithNoVariables
TEST(EvalTests, EvaluateExpressionWithNoVariables) {
    std::string input = "exp + * 2 3 4\neval 5\nprint\nend\n";
    std::string expectedOutput = "10";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}




// Expression: 5
// Derivative: 0
TEST(DifferentiationTests, Constant) {
    auto constantNode = std::make_shared<ConstantNode>("5");
    DiffVisitor diffVisitor("x");
    constantNode->accept(diffVisitor);
    auto derivative = diffVisitor.result;

    // Simplify and print
    SimplifyVisitor simplifyVisitor;
    derivative->accept(simplifyVisitor);
    derivative = simplifyVisitor.result;

    std::ostringstream out;
    derivative->printInfix(out);
    EXPECT_EQ(out.str(), "0");
}

// Expression: x^3
// Derivative: 3x^2
TEST(DifferentiationTests, PowerRule) {
    auto xNode = std::make_shared<VariableNode>("x");
    auto exponentNode = std::make_shared<ConstantNode>("3");
    auto powerNode = std::make_shared<OperatorNode>("^");
    powerNode->operands.push_back(xNode);
    powerNode->operands.push_back(exponentNode);

    DiffVisitor diffVisitor("x");
    powerNode->accept(diffVisitor);
    auto derivative = diffVisitor.result;

    SimplifyVisitor simplifyVisitor;
    derivative->accept(simplifyVisitor);
    derivative = simplifyVisitor.result;

    std::ostringstream out;
    derivative->printInfix(out);
    EXPECT_EQ(out.str(), "3*x^2");
}

// Expression: x
// Derivative: 1
TEST(DifferentiationTests, Variable) {
    auto variableNode = std::make_shared<VariableNode>("x");
    DiffVisitor diffVisitor("x");
    variableNode->accept(diffVisitor);
    auto derivative = diffVisitor.result;

    SimplifyVisitor simplifyVisitor;
    derivative->accept(simplifyVisitor);
    derivative = simplifyVisitor.result;

    std::ostringstream out;
    derivative->printInfix(out);
    EXPECT_EQ(out.str(), "1");
}

// Test Case: NotSimplified
TEST(SimplifyTests, Zerostays) {
    std::string input = "exp * 2 0\nprint\nend\n";
    std::string expectedOutput = "2*0"; 

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: Simplify
TEST(SimplifyTests, Simplified) {
    std::string input = "exp * 2 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: Complex Expression Simplified
TEST(SimplifyTests, ComplexExpressionSimplified) {
    std::string input = "exp * + * 3 1 0 + 7 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "21";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: AdditionWithZero
TEST(SimplifyTests, AdditionWithZero) {
    std::string input = "exp + x 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: AdditionWithConstants
TEST(SimplifyTests, AdditionWithConstants) {
    std::string input = "exp + 2 2\nsimp\nprint\nend\n";
    std::string expectedOutput = "4";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: AdditionWithConstantsDecimalPoints

TEST(SimplifyTests, AdditionWithConstantsDecimalPoints) {
    std::string input = "exp + 2.0002 2\nsimp\nprint\nend\n";
    std::string expectedOutput = "4.0002";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: ZeroPlusVariable
TEST(SimplifyTests, ZeroPlusVariable) {
    std::string input = "exp + 0 x\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SubtractionByZero
TEST(SimplifyTests, SubtractionByZero) {
    std::string input = "exp - x 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: VariableMinusItself
TEST(SimplifyTests, VariableMinusItself) {
    std::string input = "exp - x x\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: MultiplicationByZero
TEST(SimplifyTests, MultiplicationByZero) {
    std::string input = "exp * x 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: ZeroTimesVariable
TEST(SimplifyTests, ZeroTimesVariable) {
    std::string input = "exp * 0 x\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: MultiplicationByOne
TEST(SimplifyTests, MultiplicationByOne) {
    std::string input = "exp * x 1\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: OneTimesVariable
TEST(SimplifyTests, OneTimesVariable) {
    std::string input = "exp * 1 x\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: DivisionByOne
TEST(SimplifyTests, DivisionByOne) {
    std::string input = "exp / x 1\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: ZeroDividedByVariable
TEST(SimplifyTests, ZeroDividedByVariable) {
    std::string input = "exp / 0 x\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: VariableDividedByItself
TEST(SimplifyTests, VariableDividedByItself) {
    std::string input = "exp / x x\nsimp\nprint\nend\n";
    std::string expectedOutput = "1";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: VariableToZeroPower
TEST(SimplifyTests, VariableToZeroPower) {
    std::string input = "exp ^ x 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "1";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: VariableToOnePower
TEST(SimplifyTests, VariableToOnePower) {
    std::string input = "exp ^ x 1\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SinZero
TEST(SimplifyTests, SinZero) {
    std::string input = "exp sin 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: CosZero
TEST(SimplifyTests, CosZero) {
    std::string input = "exp cos 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "1";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: MultiplicationByZeroThenAddition
TEST(SimplifyTests, MultiplicationByZeroThenAddition) {
    std::string input = "exp + * x 0 y\nsimp\nprint\nend\n";
    std::string expectedOutput = "y";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: AdditionOfZeroThenMultiplication
TEST(SimplifyTests, AdditionOfZeroThenMultiplication) {
    std::string input = "exp * x + 0 y\nsimp\nprint\nend\n";
    std::string expectedOutput = "x*y";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SubtractionThenMultiplication
TEST(SimplifyTests, SubtractionThenMultiplication) {
    std::string input = "exp * - x x y\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: DivisionThenAddition
TEST(SimplifyTests, DivisionThenAddition) {
    std::string input = "exp + / x x y\nsimp\nprint\nend\n";
    std::string expectedOutput = "1+y";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SinZeroPlusPi
TEST(SimplifyTests, SinZeroPlusPi) {
    std::string input = "exp + sin 0 pi\nsimp\nprint\nend\n";
    std::string expectedOutput = "pi";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: CosZeroTimesVariable
TEST(SimplifyTests, CosZeroTimesVariable) {
    std::string input = "exp * cos 0 x\nsimp\nprint\nend\n";
    std::string expectedOutput = "x";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SinPi
TEST(SimplifyTests, SinPi) {
    std::string input = "exp sin pi\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SinVariable
TEST(SimplifyTests, SinVariable) {
    std::string input = "exp sin x\nsimp\nprint\nend\n";
    std::string expectedOutput = "sin(x)";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: CosVariable
TEST(SimplifyTests, CosVariable) {
    std::string input = "exp cos x\nsimp\nprint\nend\n";
    std::string expectedOutput = "cos(x)";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: MultiplicationByZeroThenAdditionWithVariable
TEST(SimplifyTests, MultiplicationByZeroThenAdditionWithVariable) {
    std::string input = "exp + * 0 x y\nsimp\nprint\nend\n";
    std::string expectedOutput = "y";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SinZeroTimesCosZero
TEST(SimplifyTests, SinZeroTimesCosZero) {
    std::string input = "exp * sin 0 cos 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: SinZeroDividedByCosZero
TEST(SimplifyTests, SinZeroDividedByCosZero) {
    std::string input = "exp / sin 0 cos 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "0";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: CosZeroDividedBySinZero
TEST(SimplifyTests, CosZeroDividedBySinZero) {
    std::string input = "exp / cos 0 sin 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "Error: Division by zero";

    EXPECT_THROW({
            std::string actualOutput = CaptureOutput(input);
        }, std::runtime_error);
}

// Test Case: DivisionByZero
TEST(SimplifyTests, DivisionByZero) {
    std::string input = "exp / 1 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "Error: Division by zero";

   
    EXPECT_THROW({
            std::string actualOutput = CaptureOutput(input);
        }, std::runtime_error);
}

// Test Case: ExpressionWithPi
TEST(SimplifyTests, ExpressionWithPi) {
    std::string input = "exp * pi 1\nsimp\nprint\nend\n";
    std::string expectedOutput = "pi";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: VariableTimesPi
TEST(SimplifyTests, VariableTimesPi) {
    std::string input = "exp * x pi\nsimp\nprint\nend\n";
    std::string expectedOutput = "x*pi";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case: ZeroRaisedToZero
TEST(SimplifyTests, ZeroRaisedToZero) {
    std::string input = "exp ^ 0 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "Error: Indeterminate form 0^0";

    EXPECT_THROW({
        std::string actualOutput = CaptureOutput(input);
        }, std::runtime_error);
}

// Test Case: ExponentiationWithZeroExponent
TEST(SimplifyTests, ExponentiationWithZeroExponent) {
    std::string input = "exp ^ x 0\nsimp\nprint\nend\n";
    std::string expectedOutput = "1";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}



// Test Case 1: Empty Input
TEST(ProgramRunnerInfixTest, EmptyInput) {
    std::string input = "";
    std::string expectedOutput = "";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 2: Simple Addition (No change, parentheses required for addition)
TEST(ProgramRunnerInfixTest, SimpleAddition) {
    std::string input = "exp + 3 4\nprint\nend\n";
    std::string expectedOutput = "3+4";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 3: Nested Expression (Parentheses needed for correct precedence)
TEST(ProgramRunnerInfixTest, NestedExpression) {
    std::string input = "exp * + 1 2 - 3 4\nprint\nend\n";
    std::string expectedOutput = "(1+2)*(3-4)";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 4: Unary Operator (No change)
TEST(ProgramRunnerInfixTest, UnaryOperator) {
    std::string input = "exp sin x\nprint\nend\n";
    std::string expectedOutput = "sin(x)";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 5: Variables and Constants (No parentheses needed for multiplication)
TEST(ProgramRunnerInfixTest, VariablesAndConstants) {
    std::string input = "exp * x pi\nprint\nend\n";
    std::string expectedOutput = "x*pi";  // No parentheses needed here

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 6: Power Operator (Parentheses needed for power)
TEST(ProgramRunnerInfixTest, PowerOperator) {
    std::string input = "exp ^ x 2\nprint\nend\n";
    std::string expectedOutput = "x^2";  // No extra parentheses needed

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 7: Function Composition (No change)
TEST(ProgramRunnerInfixTest, FunctionComposition) {
    std::string input = "exp sin cos x\nprint\nend\n";
    std::string expectedOutput = "sin(cos(x))";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 8: Negative Number (Parentheses required for negative)
TEST(ProgramRunnerInfixTest, NegativeNumber) {
    std::string input = "exp + -3 5\nprint\nend\n";
    std::string expectedOutput = "-3+5";  // Parentheses for negative number

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 9: Decimal Numbers (No parentheses needed for multiplication)
TEST(ProgramRunnerInfixTest, DecimalNumbers) {
    std::string input = "exp * 3.14 x\nprint\nend\n";
    std::string expectedOutput = "3.14*x";  // No parentheses needed

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 10: Multiple Operations (Parentheses not needed)
TEST(ProgramRunnerInfixTest, MultipleOperations) {
    std::string input = "exp + * 2 x / y 3\nprint\nend\n";
    std::string expectedOutput = "2*x+y/3";  // Parentheses not needed

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 11: Missing Expression after 'exp' (No change)
TEST(ProgramRunnerInfixTest, MissingExpressionAfterExp) {
    std::string input = "exp\nprint\nend\n";
    std::string expectedOutput = "Error: No expression provided after 'exp' command.\nError: No expression to print.";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 12: No Expression to Print (No change)
TEST(ProgramRunnerInfixTest, NoExpressionToPrint) {
    std::string input = "print\nend\n";
    std::string expectedOutput = "Error: No expression to print.";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 13: Invalid Expression (No change)
TEST(ProgramRunnerInfixTest, InvalidExpression) {
    std::string input = "exp + 1\nprint\nend\n";
    std::string expectedOutput = "Error parsing expression: Unexpected end of input\nError: No expression to print.";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 15: Cosine Expression (Parentheses for correct precedence)
TEST(ProgramRunnerInfixTest, CosineExpression) {
    std::string input = "exp cos * * pi x 2\nprint\nend\n";
    std::string expectedOutput = "cos(pi*x*2)";  // Parentheses needed to enforce precedence

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}
// Test Case 16: Only when need parentheses
TEST(ProgramRunnerInfixTest, NeedParantheses) {
    std::string input = "exp * + 2 x 5\nprint\nend\n";
    std::string expectedOutput = "(2+x)*5";  // Parentheses needed to enforce precedence

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 17: Only when need parentheses
TEST(ProgramRunnerInfixTest, DontNeedParantheses) {
    std::string input = "exp + * 2 x 5\nprint\nend\n";
    std::string expectedOutput = "2*x+5";  // Parentheses needed to enforce precedence

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 18: Only when need parentheses

TEST(ProgramRunnerInfixTest, MultipleOperationsAgainstPrecedence) {
    std::string input = "exp * + 2 x - y 3\nprint\nend\n";
    std::string expectedOutput = "(2+x)*(y-3)";  // Parentheses not needed

    std::string actualOutput = CaptureOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}



// Test Case 1: Feedback: Exponent should only be able to be a number
TEST(ParseExpressionTest, ExponentMustBeNumber) {
    // Valid case: Exponent is a number
    {
        std::queue<std::string> tokens;
        tokens.push("^");
        tokens.push("2"); // base
        tokens.push("3"); // exponent (valid)

        EXPECT_NO_THROW({
            auto result = parseExpression(tokens);
            });
    }

    // Invalid case: Exponent is an expression
    {
        std::queue<std::string> tokens;
        tokens.push("^");
        tokens.push("2"); // base
        tokens.push("+"); // invalid exponent (expression)
        tokens.push("1");
        tokens.push("1");

        EXPECT_THROW({
            auto result = parseExpression(tokens);
            }, std::runtime_error);
    }
}

// Test Case 2: Feedback: Exponent should only be able to be a number
TEST(ParseExpressionTest, ExponentMustBeNumberComplex) {
    // Valid case: Exponent is a number
    {
        std::queue<std::string> tokens;
        tokens.push("^");
        tokens.push("*"); // base
        tokens.push("+"); 
        tokens.push("a"); 
        tokens.push("b"); 
        tokens.push("c"); 
        tokens.push("2"); // exponent

        EXPECT_NO_THROW({
            auto result = parseExpression(tokens);
            });
    }
}

// Function to normalize GraphViz output by sorting nodes and edges
std::string NormalizeGraphVizOutput(const std::string& dotOutput) {
    std::vector<std::string> nodes;
    std::vector<std::string> edges;

    std::istringstream stream(dotOutput);
    std::string line;

    // Regular expressions for nodes and edges
    std::regex nodeRegex(R"(^\s*\d+\s*\[label="[^"]*"\]\s*;)");
    std::regex edgeRegex(R"(^\s*\d+\s*->\s*\d+\s*;)");

    // Process each line and categorize it
    while (std::getline(stream, line)) {
        if (std::regex_match(line, nodeRegex)) {
            nodes.push_back(line);
        }
        else if (std::regex_match(line, edgeRegex)) {
            edges.push_back(line);
        }
    }

    // Sort nodes and edges
    std::sort(nodes.begin(), nodes.end());
    std::sort(edges.begin(), edges.end());

    // Reconstruct the normalized output
    std::ostringstream normalizedOutput;
    normalizedOutput << "digraph G {\n";
    for (const auto& node : nodes) {
        normalizedOutput << node << "\n";
    }
    for (const auto& edge : edges) {
        normalizedOutput << edge << "\n";
    }
    normalizedOutput << "}";

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

// Test Case 1: Empty Input
TEST(ProgramRunnerGraphVizTest, EmptyInput) {
    std::string input = "";
    std::string expectedOutput = "";

    std::string actualOutput = CaptureGraphVizOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 2: Simple Addition
TEST(ProgramRunnerGraphVizTest, SimpleAddition) {
    std::string input = "exp + 3 4\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput =
        "digraph G {\n"
        "1 [label=\"+\"];\n"
        "2 [label=\"3\"];\n"
        "1 -> 2;\n"
        "3 [label=\"4\"];\n"
        "1 -> 3;\n"
        "}";

    std::string actualOutput = CaptureGraphVizOutput(input);
    EXPECT_EQ(actualOutput, expectedOutput);
}

// Test Case 3: Nested Expression
TEST(ProgramRunnerGraphVizTest, NestedExpression) {
    std::string input = "exp * + 1 2 - 3 4\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput =
        "digraph G {\n"
        "1 [label=\"*\"];\n"
        "2 [label=\"+\"];\n"
        "1 -> 2;\n"
        "3 [label=\"1\"];\n"
        "2 -> 3;\n"
        "4 [label=\"2\"];\n"
        "2 -> 4;\n"
        "5 [label=\"-\"];\n"
        "1 -> 5;\n"
        "6 [label=\"3\"];\n"
        "5 -> 6;\n"
        "7 [label=\"4\"];\n"
        "5 -> 7;\n"
        "}";
    std::string actualOutput = CaptureGraphVizOutput(input);

    std::string normalizedActual = NormalizeGraphVizOutput(actualOutput);
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);

    EXPECT_EQ(normalizedActual, normalizedExpected);
}

// Test Case 4: Expression with Constants and Variables
TEST(ProgramRunnerGraphVizTest, ConstantsAndVariables) {
    std::string input = "exp + x * 2 pi\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput =
        "digraph G {\n"
        "1 [label=\"+\"];\n"
        "2 [label=\"x\"];\n"
        "1 -> 2;\n"
        "3 [label=\"*\"];\n"
        "1 -> 3;\n"
        "4 [label=\"2\"];\n"
        "3 -> 4;\n"
        "5 [label=\"&pi;\"];\n"
        "3 -> 5;\n"
        "}";

    std::string actualOutput = CaptureGraphVizOutput(input);

    std::string normalizedActual = NormalizeGraphVizOutput(actualOutput);
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);
    EXPECT_EQ(normalizedActual, normalizedExpected);
}


// Test Case 5: Single Node (No Operations)
TEST(ProgramRunnerGraphVizTest, SingleNode) {
    std::string input = "exp 42\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput =
        "digraph G {\n"
        "1 [label=\"42\"];\n"
        "}";

    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedActual = NormalizeGraphVizOutput(actualOutput);
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);
    EXPECT_EQ(normalizedActual, normalizedExpected);
}

// Test Case 6: Complex Nested Expression
TEST(ProgramRunnerGraphVizTest, ComplexNestedExpression) {
    std::string input = "exp + * 1 2 / - 4 3 5\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput =
        "digraph G {\n"
        "1 [label=\"+\"];\n"
        "2 [label=\"*\"];\n"
        "1 -> 2;\n"
        "3 [label=\"1\"];\n"
        "2 -> 3;\n"
        "4 [label=\"2\"];\n"
        "2 -> 4;\n"
        "5 [label=\"/\"];\n"
        "1 -> 5;\n"
        "6 [label=\"-\"];\n"
        "5 -> 6;\n"
        "7 [label=\"4\"];\n"
        "6 -> 7;\n"
        "8 [label=\"3\"];\n"
        "6 -> 8;\n"
        "9 [label=\"5\"];\n"
        "5 -> 9;\n"
        "}";

    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedActual = NormalizeGraphVizOutput(actualOutput);
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);
    EXPECT_EQ(normalizedActual, normalizedExpected);
}

// Test Case 7: Expression with Multiple Variables
TEST(ProgramRunnerGraphVizTest, MultipleVariables) {
    std::string input = "exp * a + b c\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput =
        "digraph G {\n"
        "1 [label=\"*\"];\n"
        "2 [label=\"a\"];\n"
        "1 -> 2;\n"
        "3 [label=\"+\"];\n"
        "1 -> 3;\n"
        "4 [label=\"b\"];\n"
        "3 -> 4;\n"
        "5 [label=\"c\"];\n"
        "3 -> 5;\n"
        "}";

    std::string actualOutput = CaptureGraphVizOutput(input);
    std::string normalizedActual = NormalizeGraphVizOutput(actualOutput);
    std::string normalizedExpected = NormalizeGraphVizOutput(expectedOutput);
    EXPECT_EQ(normalizedActual, normalizedExpected);
}

// Test Case 8: Missing Operand
TEST(ProgramRunnerGraphVizTest, MissingOperand) {
    std::string input = "exp + 3\ndot " + TEST_FILE_NAME + "\nend\n";
    std::string expectedOutput = "Error: No expression to output.";

    std::string actualOutput = CaptureOutput(input);
    EXPECT_TRUE(actualOutput.find("Error") != std::string::npos);
}