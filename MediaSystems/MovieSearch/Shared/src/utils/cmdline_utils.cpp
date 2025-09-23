/**
* author Yme Brugts (s4536622)
 * @file cmdline_utils.cpp
 * @date 2025-09-16
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

    std::vector<std::string> tokenize_command_line(const std::string& terminal_input) {
        std::vector<std::string> tokens;
        std::string current_token;

        for (char input_char : terminal_input) {
            if (std::isspace(static_cast<unsigned char>(input_char))) {
                if (!current_token.empty()) {
                    tokens.emplace_back(std::move(current_token));
                    current_token.clear();
                }
            }
            else {
                current_token.push_back(input_char);
            }
        }

        if (!current_token.empty()) {
            tokens.emplace_back(std::move(current_token));
        }

        return tokens;
    }

}