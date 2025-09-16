/**
 * author Yme Brugts (s4536622)
 * @file simplify_visitor.cpp
 * @date 2024-10-28
 */

#include <limits>
#include "double_utils.h"


 /**
  * @function formatDouble
  * @abstract Formats a double value into a string with full precision, removing trailing zeros and unnecessary decimal points. 
    Digits10 was chosen because it allows to retain as much precision as possible without getting the double inaccuracies 
  * @param value The double value to format.
  * @return A string representing the formatted double.
  */
std::string formatDouble(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(std::numeric_limits<double>::digits10) << value;

    std::string str = oss.str();

    // Remove trailing zeros and possible decimal point
    if (str.find('.') != std::string::npos) {
        //Find last non zero character and trim the rest
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        // Remove the decimal point if it's the last character
        if (str.back() == '.') {
            str.pop_back();
        }
    }

    return str;
}