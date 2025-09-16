/**
* author Yme Brugts (s4536622)
 * @file cmdline_utils.cpp
 * @date 2025-01-16
 */

#include "cmdline_utils.h"

namespace shared::utils {

    bool token_is_option(const std::string& t) {
        return t.size() >= 2 && t[0] == '-';
    }

    bool matches_option(const std::string& tok, const std::string& name) {
        if (tok.size() < 2 || tok[0] != '-') return false;
        std::size_t i = (tok.size() >= 3 && tok[1] == '-') ? 2 : 1;
        return tok.substr(i) == name;
    }

    std::pair<std::string, std::string> split_opt_equals(const std::string& tok) {
        auto p = tok.find('=');
        if (p == std::string::npos) return { tok, "" };
        return { tok.substr(0, p), tok.substr(p + 1) };
    }

    std::vector<std::string> collect_value_tokens(const std::vector<std::string>& tokens, std::size_t& i) {
        std::vector<std::string> vals;
        while (i < tokens.size() && !token_is_option(tokens[i])) {
            if (!tokens[i].empty()) vals.push_back(tokens[i]);
            ++i;
        }
        return vals;
    }

}