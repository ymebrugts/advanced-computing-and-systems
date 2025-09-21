#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file cmdline_utils.h
 * @date 2025-09-16
 */

#include <string>
#include <vector>
#include <utility>

namespace shared::utils {

    /**
     * @brief Check if a token is a command line option (starts with -)
     * @param t Token to check
     * @return true if token is an option, false otherwise
     */
    bool token_is_option(const std::string& t);

    /**
     * @brief Check if a token matches a specific option name
     * @param tok Token to check
     * @param name Option name to match against
     * @return true if token matches the option name
     */
    bool matches_option(const std::string& tok, const std::string& name);

    /**
     * @brief Split an option token on '=' character
     * @param tok Token to split
     * @return Pair of option and value (value empty if no '=' found)
     */
    std::pair<std::string, std::string> split_opt_equals(const std::string& tok);

    /**
     * @brief Collect contiguous non-option tokens as separate items
     * @param tokens Vector of tokens
     * @param i Reference to current index (will be modified)
     * @return Vector of value tokens
     */
    std::vector<std::string> collect_value_tokens(const std::vector<std::string>& tokens, std::size_t& i);

}