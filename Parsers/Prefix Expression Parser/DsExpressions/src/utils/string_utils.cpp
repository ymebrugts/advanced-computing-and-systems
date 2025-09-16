/**
 * author Yme Brugts (s4536622)
 * @file simplify_visitor.cpp
 * @date 2024-10-27
 */

#include "string_utils.h"

#include <sstream>
#include <string>
#include <iomanip>
#include <limits>

/**
 * @function is_number
 * @abstract Validates whether a given string represents a properly formatted number, including support for integers, decimals, and optional negative signs.
 * @param s The string (`const std::string&`) to validate as a number.
 * @return A boolean (`bool`) indicating if the string `s` is a valid number.
 * @pre The string `s` may contain a sequence of characters to be evaluated as a potential number.
 * @post - The function returns `true` if `s` is a valid number, consisting of an optional negative sign, digits, and an optional single decimal point.
 *       - The function returns `false` if `s` contains invalid characters, multiple decimal points, or other non-numeric symbols.
 **/
bool is_number(const std::string& s) {
    enum State { START, INT, FRAC };
    State state = START;
    bool has_digits = false;

    for (char c : s) {
        switch (state) {
        case START:
            if (c == '-') state = INT;
            else if (std::isdigit(c)) { state = INT; has_digits = true; }
            else return false;
            break;
        case INT:
            if (std::isdigit(c)) has_digits = true;
            else if (c == '.') state = FRAC;
            else return false;
            break;
        case FRAC:
            if (std::isdigit(c)) has_digits = true;
            else return false;
            break;
        }
    }

    return has_digits && (state == INT || state == FRAC);
}

/**
 * @function toFullPrecisionString
 * @abstract Converts a `double` to a string with maximum precision, preserving all significant digits to ensure accuracy.
 * @param value The `double` value to convert to a string with full precision.
 * @return A `std::string` representing the `double` value with maximal precision.
 * @pre The `value` is a valid `double` to be represented accurately as a string.
 * @post - The function returns a string with all significant digits of `value`, using `std::defaultfloat` to avoid unnecessary trailing zeros or scientific notation where possible.
 *       - The output string retains enough precision to uniquely identify the original `double` value.
 **/
std::string toFullPrecisionString(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
    return oss.str();
}