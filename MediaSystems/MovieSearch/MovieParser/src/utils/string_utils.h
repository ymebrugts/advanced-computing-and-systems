/**
* author Yme Brugts (s4536622)
 * @file string_utils.h
 * @date 2025-01-16
 */

#pragma once
/**
 * @file string_utils.h
 * @brief String manipulation utilities
 */

#include <string>

namespace moviesearch::utils {

    /**
     * @brief Remove leading whitespace from a string
     * @param s Input string
     * @return String with leading whitespace removed
     */
    std::string ltrim(std::string s);

    /**
     * @brief Remove trailing whitespace from a string
     * @param s Input string
     * @return String with trailing whitespace removed
     */
    std::string rtrim(std::string s);

    /**
     * @brief Remove leading and trailing whitespace from a string
     * @param s Input string
     * @return String with leading and trailing whitespace removed
     */
    std::string trim(std::string s);

    /**
     * @brief Replace all occurrences of a substring in a string
     * @param s String to modify
     * @param from Substring to replace
     * @param to Replacement substring
     */
    void replaceAll(std::string& s, const std::string& from, const std::string& to);

    /**
     * @brief Normalize UTF-8 en/em dashes to ASCII hyphen-minus
     * @param s String to normalize
     */
    void normalize_dashes(std::string& s);

}