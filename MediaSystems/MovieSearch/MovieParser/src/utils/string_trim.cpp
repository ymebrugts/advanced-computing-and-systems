/**
* author Yme Brugts (s4536622)
 * @file string_trim.cpp
 * @date 2024-12-02
 */

#include "string_trim.h"

/**
 * Trims leading and trailing whitespace characters from a string.
 *
 * This function removes spaces and tab characters from the beginning
 * and end of the provided string, effectively 'trimming' the string.
 *
 * @param str The string to be trimmed. The original string remains unaltered.
 * @return A new string with leading and trailing whitespace removed. If the
 *         input string is entirely composed of whitespace, an empty string
 *         is returned.
 */
std::string string_trim(const std::string& str) {
    size_t start_pos = str.find_first_not_of(" \t");
    size_t end_pos = str.find_last_not_of(" \t");
    if (start_pos != std::string::npos && end_pos != std::string::npos) {
        return str.substr(start_pos, end_pos - start_pos + 1);
    }
    else {
        return "";
    }
}